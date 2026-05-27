<div align="center">

<!-- Animated Header Banner -->
<img src="https://capsule-render.vercel.app/api?type=waving&color=0:0d1117,50:0a3d62,100:1a7fd4&height=220&section=header&text=DICOM%20Viewer&fontSize=65&fontColor=ffffff&fontAlignY=38&desc=Professional%20Medical%20Imaging%20Desktop%20Application&descAlignY=58&descSize=18&animation=fadeIn" width="100%"/>

<br/>

<!-- Tech Badges Row 1 -->
<img src="https://img.shields.io/badge/Qt-6.8+-41CD52?style=for-the-badge&logo=qt&logoColor=white" />
&nbsp;
<img src="https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" />
&nbsp;
<img src="https://img.shields.io/badge/CMake-3.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white" />
&nbsp;
<img src="https://img.shields.io/badge/DCMTK-3.6+-0057A8?style=for-the-badge&logoColor=white" />
&nbsp;
<img src="https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-555555?style=for-the-badge&logo=linux&logoColor=white" />

<br/><br/>

<!-- Status Badges Row 2 -->
<img src="https://img.shields.io/badge/Status-Active%20Development-brightgreen?style=flat-square&logo=github" />
&nbsp;
<img src="https://img.shields.io/badge/Version-1.1.0-2196F3?style=flat-square" />
&nbsp;
<img src="https://img.shields.io/badge/License-Educational-FF9800?style=flat-square" />
&nbsp;
<img src="https://img.shields.io/badge/PRs-Welcome-blueviolet?style=flat-square" />
&nbsp;
<img src="https://img.shields.io/badge/Stars-⭐%20Give%20a%20Star!-yellow?style=flat-square" />

<br/><br/>

**[🔬 Features](#-features) &nbsp;·&nbsp; [🆕 What's New](#-whats-new) &nbsp;·&nbsp; [🛠️ Tech Stack](#%EF%B8%8F-tech-stack) &nbsp;·&nbsp; [🏗️ Architecture](#%EF%B8%8F-architecture) &nbsp;·&nbsp; [🚀 Getting Started](#-getting-started) &nbsp;·&nbsp; [🗺️ Roadmap](#%EF%B8%8F-roadmap)**

<br/>

</div>

---

## 🏥 What is DicomViewer?

**DicomViewer** is a professional-grade desktop application for loading, viewing, and analyzing **DICOM medical imaging studies** — CT scans, MRIs, X-Rays, and more. Built entirely from scratch using **Qt 6 + QML** for the UI and **DCMTK** for DICOM parsing, it mirrors the core workflow of real PACS workstations used in hospitals and radiology departments.

<div align="center">

```
╔═══════════════════════════════════════════════════════════════╗
║        CT Scan  ──►  Load Study  ──►  Navigate Slices        ║
║        MRI Scan ──►  Metadata    ──►  Window / Level         ║
║        X-Ray    ──►  Zoom & Pan  ──►  Clinical Presets       ║
╚═══════════════════════════════════════════════════════════════╝
```

</div>

> 💡 **Learning Focus:** Qt/QML architecture · DICOM protocols · Medical slice ordering · Image rendering pipelines · Windowing algorithms · Desktop UI development

<br/>

---

## 🆕 What's New — Proper Medical Slice Sorting

> **v1.1.0 — The most critical upgrade for medical correctness**

Previously, DICOM files were loaded in **filesystem folder order** — which is arbitrary and medically wrong. Files named `10.dcm`, `1.dcm`, `2.dcm` would load in that exact broken order.

Now, DicomViewer loads slices in **correct anatomical order** using real DICOM metadata. This was achieved by introducing **3 new components**:

---

### 1. `DicomSliceInfo` — Slice Metadata Struct

```cpp
struct DicomSliceInfo
{
    QString filePath;
    int     instanceNumber = 0;
    double  sliceLocation  = 0.0;
};
```

A lightweight struct that holds **3 pieces of information** for every DICOM file discovered in the folder:

| Field | DICOM Tag | Meaning |
|:---|:---:|:---|
| `filePath` | — | Full path to the `.dcm` file on disk |
| `instanceNumber` | `DCM_InstanceNumber` | The slice's sequential number in the series |
| `sliceLocation` | `DCM_SliceLocation` | The physical Z-position of the slice in mm |

**Example — a brain CT with 3 slices:**

```
brain1.dcm  →  { filePath: "brain1.dcm",  instanceNumber: 1,  sliceLocation: 0.0  }
brain2.dcm  →  { filePath: "brain2.dcm",  instanceNumber: 2,  sliceLocation: 5.0  }
brain3.dcm  →  { filePath: "brain3.dcm",  instanceNumber: 3,  sliceLocation: 10.0 }
```

---

### 2. `readSliceInfo()` — Metadata Reader

```cpp
DicomSliceInfo readSliceInfo(const QString& fullPath);
```

For each DICOM file, this function opens the dataset with DCMTK and extracts the sorting keys:

```cpp
// Read slice number from DICOM header
Sint32 instanceNumber = 0;
dataset->findAndGetSint32(DCM_InstanceNumber, instanceNumber);

// Read physical slice position from DICOM header
Float64 sliceLocation = 0.0;
dataset->findAndGetFloat64(DCM_SliceLocation, sliceLocation);
```

This runs **before** any pixel data is loaded — it's a lightweight metadata-only pass over all files.

---

### 3. `std::sort()` — Anatomical Order Sorting

```cpp
std::sort(slices.begin(), slices.end(),
    [](const DicomSliceInfo& a, const DicomSliceInfo& b) {
        return a.instanceNumber < b.instanceNumber;
    });
```

After all slice metadata is collected, slices are **sorted by `InstanceNumber`** before images are loaded.

**Before sorting** — folder order (wrong ❌)
```
10.dcm  →  Slice 10  (loaded first!)
1.dcm   →  Slice 1
2.dcm   →  Slice 2
```

**After sorting** — medical order (correct ✅)
```
1.dcm   →  Slice 1
2.dcm   →  Slice 2
10.dcm  →  Slice 10
```

---

### 🔄 Updated Loading Pipeline

```
  ┌──────────────────────────────────────────────────────────────┐
  │              NEW SLICE-SORTED LOADING PIPELINE               │
  ├──────────────────────────────────────────────────────────────┤
  │                                                              │
  │   📁  Scan DICOM Folder                                      │
  │             │                                                │
  │             ▼                                                │
  │   📋  readSliceInfo()   ◄── Read InstanceNumber +            │
  │             │                SliceLocation (metadata only)   │
  │             ▼                                                │
  │   🔢  std::sort()       ◄── Sort by instanceNumber ASC       │
  │             │                                                │
  │             ▼                                                │
  │   🖼️  Load images in    ◄── Pixel data read in correct       │
  │        sorted order          anatomical order                │
  │             │                                                │
  │             ▼                                                │
  │   📦  m_images[]        ◄── Medically correct slice array    │
  │                                                              │
  └──────────────────────────────────────────────────────────────┘
```

> ⚕️ **Why does this matter?** Correct slice order is **essential** for Coronal / Sagittal reconstruction, MPR (Multiplanar Reconstruction), Cine playback, and future 3D volume rendering. Without it, body reconstruction produces anatomically broken results.

<br/>

---

## ✨ Features

<table>
<tr>
<td width="50%" valign="top">

### 🗂️ Study Management
```
📁  Load full DICOM study from folder
📄  Supports .dcm + multi-slice series
🔢  Medical slice order (NEW ✅)
🏷️  Patient metadata panel — 10+ fields

    • Patient Name & Patient ID
    • Study Date & Modality
    • Study & Series Description
    • Institution Name
    • Slice Thickness & Pixel Spacing
    • Image Resolution
```

</td>
<td width="50%" valign="top">

### 🖼️ Viewer Interactions
```
🖱️  Mouse Wheel       →  Scroll slices
⌨️  Ctrl + Wheel     →  Zoom in / out
✋  Mouse Drag        →  Pan image
🔄  Reset Button      →  Reset view
⬅️  Prev / Next ▶    →  Slice controls
```

</td>
</tr>
<tr>
<td width="50%" valign="top">

### 🎛️ Windowing Controls
```
↔️  Window Width   →  Contrast control
↕️  Window Level   →  Brightness control

🦴  Bone    (WW: 2000  /  WL:  400)
🫁  Lung    (WW: 1500  /  WL: -600)
🧠  Brain   (WW:   80  /  WL:   40)
```

</td>
<td width="50%" valign="top">

### 🖥️ Professional UI
```
🌑  Dark PACS-style medical theme
📊  Side information metadata panel
⚡  Anti-flicker image version engine
🔁  Real-time WW/WL re-rendering
🎨  Smooth, high-quality rendering
```

</td>
</tr>
</table>

<br/>

---

## 🛠️ Tech Stack

<div align="center">

### 🔷 Core Framework

<table>
<tr>
<td align="center" width="25%">
<img src="https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white"/><br/>
<b>Qt 6.8+</b><br/>
<sub>Desktop App Framework</sub>
</td>
<td align="center" width="25%">
<img src="https://img.shields.io/badge/QML-41CD52?style=for-the-badge&logo=qt&logoColor=white"/><br/>
<b>QML (Qt 6)</b><br/>
<sub>Declarative UI Layer</sub>
</td>
<td align="center" width="25%">
<img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white"/><br/>
<b>C++ 17</b><br/>
<sub>Backend & Image Logic</sub>
</td>
<td align="center" width="25%">
<img src="https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white"/><br/>
<b>CMake 3.20+</b><br/>
<sub>Build System</sub>
</td>
</tr>
</table>

<br/>

### 🔶 Libraries & Tools

<table>
<tr>
<td align="center" width="25%">
<img src="https://img.shields.io/badge/DCMTK-0057A8?style=for-the-badge&logoColor=white"/><br/>
<b>DCMTK 3.6+</b><br/>
<sub>DICOM Parsing & Rendering</sub>
</td>
<td align="center" width="25%">
<img src="https://img.shields.io/badge/vcpkg-5C2D91?style=for-the-badge&logo=microsoft&logoColor=white"/><br/>
<b>vcpkg</b><br/>
<sub>Windows Dependency Mgr</sub>
</td>
<td align="center" width="25%">
<img src="https://img.shields.io/badge/GCC%20%2F%20Clang-A42E2B?style=for-the-badge&logo=gnu&logoColor=white"/><br/>
<b>GCC 11+ / Clang 14+</b><br/>
<sub>Linux Compilers</sub>
</td>
<td align="center" width="25%">
<img src="https://img.shields.io/badge/MSVC-0078D7?style=for-the-badge&logo=visualstudio&logoColor=white"/><br/>
<b>MSVC 2022</b><br/>
<sub>Windows Compiler</sub>
</td>
</tr>
</table>

</div>

<br/>

---

## 🏗️ Architecture

### 📂 Project Structure

```
DicomViewer/
│
├── 📂 backend/
│   ├── 🔧 dicomcontroller.h/.cpp     ← QML ↔ C++ bridge (Q_PROPERTY, Signals & Slots)
│   ├── 📖 dicomloader.h/.cpp         ← DICOM parsing, slice sorting, WW/WL rendering
│   │       ├── DicomSliceInfo        ←   NEW: per-file metadata struct
│   │       ├── readSliceInfo()       ←   NEW: reads InstanceNumber + SliceLocation
│   │       └── std::sort()           ←   NEW: sorts slices into anatomical order
│   └── 🖼️  dicomimageprovider.h/.cpp  ← QQuickImageProvider → streams QImage to QML
│
├── 🎨 Main.qml                       ← Application UI (dark PACS medical theme)
├── ⚙️  main.cpp                       ← Entry point, QML engine & provider registration
└── 📋 CMakeLists.txt                 ← Cross-platform build configuration
```

### 🔄 Full Data Flow Pipeline

```
  ┌────────────────────────────────────────────────────────────┐
  │                 DICOM RENDERING PIPELINE                   │
  ├────────────────────────────────────────────────────────────┤
  │                                                            │
  │   📁  Open DICOM Folder                                    │
  │             │                                              │
  │             ▼                                              │
  │   🎮  DicomController     ◄── Q_INVOKABLE / Q_PROPERTY     │
  │             │                  (QML ↔ C++ bridge)          │
  │             ▼                                              │
  │   📋  readSliceInfo()     ◄── Reads InstanceNumber &       │
  │             │                  SliceLocation per file      │
  │             ▼                                              │
  │   🔢  std::sort()         ◄── Anatomical order guaranteed  │
  │             │                                              │
  │             ▼                                              │
  │   📖  DicomLoader         ◄── DCMTK reads pixel data       │
  │             │                  in sorted order             │
  │             ▼                                              │
  │   🖼️   QImage Generated    ◄── WW/WL algorithm applied     │
  │             │                  8-bit grayscale output      │
  │             ▼                                              │
  │   🔌  DicomImageProvider  ◄── Registered: "image://dicom/" │
  │             │                                              │
  │             ▼                                              │
  │   📺  QML Image Viewer    ◄── ?v=imageVersion              │
  │                                (prevents QML caching)      │
  └────────────────────────────────────────────────────────────┘
```

### 🧩 Component Responsibilities

| Component | Layer | Responsibility |
|:---|:---:|:---|
| `DicomSliceInfo` | 📋 Model | Stores `filePath`, `instanceNumber`, `sliceLocation` per DICOM file |
| `readSliceInfo()` | 🔍 Reader | Lightweight metadata pass — reads DICOM tags before pixel loading |
| `std::sort()` | 🔢 Sorter | Sorts slice array by `InstanceNumber` into correct anatomical order |
| `DicomLoader` | 📦 Data | Loads pixel buffers from sorted slice list, manages `m_images[]` |
| `DicomController` | 🌉 Bridge | Exposes state & actions to QML via `Q_PROPERTY` + signals/slots |
| `DicomImageProvider` | 🔌 Provider | Implements `QQuickImageProvider` to serve `QImage` objects to QML |

<br/>

---

## 🚀 Getting Started

### Prerequisites

| Tool | Version | Download |
|:---|:---:|:---|
| <img src="https://img.shields.io/badge/Qt-41CD52?style=flat&logo=qt&logoColor=white" /> Qt | 6.8+ | [qt.io/download](https://www.qt.io/download) |
| <img src="https://img.shields.io/badge/CMake-064F8C?style=flat&logo=cmake&logoColor=white" /> CMake | 3.20+ | [cmake.org/download](https://cmake.org/download/) |
| <img src="https://img.shields.io/badge/DCMTK-0057A8?style=flat" /> DCMTK | 3.6+ | [dicom.offis.de](https://dicom.offis.de/dcmtk.php.en) |
| <img src="https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white" /> Compiler | C++17 | GCC 11+ / Clang 14+ / MSVC 2022 |

---

### 🐧 Build on Linux

```bash
# 1. Clone the repository
git clone https://github.com/yourusername/DicomViewer.git
cd DicomViewer

# 2. Configure & build
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)

# 3. Run
./DicomViewer
```

### 🪟 Build on Windows (vcpkg)

```powershell
# 1. Clone the repository
git clone https://github.com/yourusername/DicomViewer.git
cd DicomViewer

# 2. Configure with vcpkg toolchain
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

# 3. Build release binary
cmake --build . --config Release
```

---

### 📂 Sample DICOM Datasets

No DICOM files on hand? Start with these free public sources:

| 🏥 Source | 🔗 Link | 📝 Description |
|:---|:---|:---|
| **TCIA** | [cancerimagingarchive.net](https://www.cancerimagingarchive.net) | Large curated cancer imaging archive |
| **OsiriX** | [osirix-viewer.com](https://www.osirix-viewer.com/resources/dicom-image-library/) | Classic DICOM sample sets (CT, MRI) |
| **DICOM Library** | [dicomlibrary.com](https://www.dicomlibrary.com) | Upload & browse shared DICOM files |
| **Kaggle** | [kaggle.com/datasets](https://www.kaggle.com/datasets?search=dicom) | Community medical imaging datasets |

<br/>

---

## 🧠 Key Concepts Demonstrated

<div align="center">

```
┌──────────────────────┬──────────────────────────────────────────────────┐
│  Qt/QML Architecture │  QObject · Q_PROPERTY · Signals & Slots          │
├──────────────────────┼──────────────────────────────────────────────────┤
│  DICOM Slice Sorting │  InstanceNumber · SliceLocation · std::sort()    │
├──────────────────────┼──────────────────────────────────────────────────┤
│  Image Pipeline      │  QQuickImageProvider · QImage · pixel buffers    │
├──────────────────────┼──────────────────────────────────────────────────┤
│  DICOM Standard      │  Metadata tags · pixel data · multi-frame series │
├──────────────────────┼──────────────────────────────────────────────────┤
│  Medical Imaging     │  WW/WL algorithm · Hounsfield units · PACS       │
├──────────────────────┼──────────────────────────────────────────────────┤
│  Desktop UI Dev      │  Responsive layouts · dark theme · caching       │
└──────────────────────┴──────────────────────────────────────────────────┘
```

</div>

<br/>

---

## 🗺️ Roadmap

### ✅ Completed
- [x] DICOM folder loading (multi-slice support)
- [x] Slice navigation (mouse wheel + prev/next controls)
- [x] Zoom & Pan with one-click reset
- [x] Real-time Window Width / Window Level controls
- [x] Clinical presets — Bone · Lung · Brain
- [x] Patient metadata extraction (10+ DICOM tags)
- [x] Professional dark PACS-style UI
- [x] **Proper medical slice sorting** — `DicomSliceInfo` · `readSliceInfo()` · `std::sort()` ✨

### 🔜 In Progress / Planned
- [ ] 🖼️ Thumbnail sidebar for quick slice overview
- [ ] 🧩 Multiplanar Reconstruction (MPR) — Axial / Sagittal / Coronal
- [ ] ▶️ Cine loop / auto-play mode
- [ ] 📏 Measurement tools — length, angle, area
- [ ] 🖊️ Annotation & markup layer
- [ ] 📦 Multiple series support in one viewer
- [ ] 🌐 PACS networking — C-FIND / C-MOVE / C-STORE
- [ ] 📤 Export to PNG / JPEG / PDF
- [ ] ⚡ Performance optimization — lazy loading for large studies
- [ ] 🔬 DICOM structured reports support

<br/>

---

## 👨‍💻 Author

<div align="center">

<img src="https://img.shields.io/badge/Built%20by-Yashwanth%20RV-0a3d62?style=for-the-badge&logo=github&logoColor=white" />

<br/><br/>

*Building real-world medical imaging software to master **Qt · C++ · DICOM** from first principles.*

<br/>

[![GitHub](https://img.shields.io/badge/GitHub-Follow-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/yourusername)
&nbsp;&nbsp;
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Connect-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://linkedin.com/in/yourusername)

</div>

<br/>

---

## 📄 License

This project is built for **educational and learning purposes**.
Free to explore, fork, and build upon — contributions and feedback welcome!

---

<div align="center">

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:1a7fd4,50:0a3d62,100:0d1117&height=120&section=footer" width="100%"/>

<sub>⚕️ Built with passion for medical imaging &nbsp;·&nbsp; Qt &nbsp;·&nbsp; QML &nbsp;·&nbsp; C++ &nbsp;·&nbsp; DCMTK</sub>

</div>
