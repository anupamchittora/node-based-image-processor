
# 🎨 Node-Based Image Manipulation Interface

An interactive visual image processing tool built with **ImGui**, **OpenGL**, and **OpenCV**. This project lets users construct complex image transformation pipelines using a **node graph interface**, where each node represents a specific image operation such as filtering, color adjustments, or edge detection.

---

## 🚀 Features Implemented

### 🔧 Core System
- **Drag-and-drop Node Canvas** with Bézier curve connections
- **Visual node linking system** with real-time processing
- **Scrollable and pannable canvas** for large graphs
- **Connection deletion** via right-click on links
- **Node selection with properties panel**
- **Node spawning toolbox** for dynamic graph construction

### 🖼️ Basic Nodes
- **Image Input Node**
  - Load JPG, PNG, BMP
  - Preview image inside node
  - Display metadata and individual "Open" button
- **Output Node**
  - Live image preview
  - Export image with format and quality options
- **Brightness/Contrast Node**
  - Two sliders and reset buttons
- **Color Channel Splitter**
  - Outputs individual R, G, B (and optional A) channels

### 🧪 Intermediate Nodes
- **Gaussian Blur Node**
  - Adjustable radius
- **Threshold Node**
  - Binary threshold with intensity control
- **Edge Detection Node**
  - Sobel & Canny options
  - Kernel size and threshold sliders
- **Blend Node**
  - 2 input support
  - Blend modes: Normal, Multiply, Screen, Overlay, Difference
  - Opacity control

### 🌱 Advanced Nodes
- **Noise Generator Node**
  - Perlin-like noise output (grayscale)
  - Customizable width, height, scale, and seed
- **Convolution Filter Node**
  - Editable 3×3 or 5×5 kernels
  - Built-in presets: Sharpen, Edge, Emboss

---

## 🛠️ Build Instructions

### ✅ Prerequisites
- **Visual Studio 2019/2022**
- **CMake** (if not using VS project directly)
- **vcpkg** (used for dependencies)

### 📦 Dependencies (Installed via `vcpkg`)
```bash
vcpkg install opencv4
vcpkg install glfw3
vcpkg install glew
vcpkg install imgui[glfw-binding,opengl3-binding]
vcpkg install boost
```

### 📁 Folder Structure
```
/node-based-image-editor
│
├── include/
│   ├── BaseNode.h, OutputNode.h, ImageInputNode.h, etc.
│   └── filters/
│       └── *.h (node headers like BlurNode.h)
│
├── src/
│   ├── App.cpp, NodeGraph.cpp, NodeUI.cpp, etc.
│   └── filters/
│       └── *.cpp (node implementations)
│
├── assets/         # optional test images
├── CMakeLists.txt  # if using cmake
├── main.cpp
├── README.md
```

### 🧰 Visual Studio Build
1. Clone project into a local folder.
2. Set include and library directories in Visual Studio:
   - C/C++ → Additional Include Directories:
     ```
     C:\vcpkg\installed\x64-windows\include
     ```
   - Linker → Additional Library Directories:
     ```
     C:\vcpkg\installed\x64-windows\lib
     ```
3. Add required `.lib` files to Linker → Input → Additional Dependencies:
   ```
   opencv_world4xx.lib
   glfw3.lib
   glew32.lib
   ```
4. Build and run!

---

## 📸 Screenshots

> _Include screenshots here showing the node editor with an image loaded, nodes connected, and effects applied._
> ![Screenshot 2025-04-15 145804](https://github.com/user-attachments/assets/bef9f5c7-11b1-4b69-be9b-40dbfb291aa9)
![Screenshot 2025-04-15 145835](https://github.com/user-attachments/assets/d203211a-a35a-4b12-8130-de63be459c50)

![Screenshot 2025-04-15 150113](https://github.com/user-attachments/assets/9876e912-bc2d-4653-bade-20d0a5671e6d)

---

## 🧩 Future Improvements
- Node deletion
- Save/load node graph to JSON
- Histogram display for thresholding
- Image comparison (before/after toggle)
- Tooltips or inline help for filters

---

## 👤 Authors

> Anupam Chittora
> [[GitHub Repository Link](https://github.com/anupamchittora/node-based-image-processor)]
