
# Design Document

## Architecture Overview

The application is designed as a node-based image editor using ImGui for the user interface, OpenGL for rendering, and OpenCV for image processing. Each operation (e.g., blur, brightness adjustment, edge detection) is encapsulated as a distinct node that can be connected to other nodes to form an image processing pipeline.

### Major Components:
1. **BaseNode**: Abstract class representing a generic processing node.
2. **NodeUIManager**: Handles node layout, dragging, and user interaction.
3. **NodeGraph**: Manages the logical connection and data flow between nodes.
4. **Render System**: Renders nodes, connections, and handles scrollable canvas via ImGui.
5. **Filters**: Custom implementations of image operations such as BlurNode, EdgeDetectionNode, etc.
6. **NodeFactory**: Creates nodes dynamically from a dropdown menu using type strings.
7. **ImageInputNode and OutputNode**: Entry and exit points for image loading and saving.
8. **TinyFileDialogs**: For file open/save dialogs.

## Data Flow

The data flows from image input through a graph of interconnected nodes. Each node, when triggered, processes its input image and passes the output to connected nodes. The system supports automatic processing upon establishing a connection and manual refresh when parameters are adjusted.

## Interaction Model

- Users can drag and drop nodes in a canvas.
- Nodes can be connected by clicking and dragging between input/output pins.
- Each node has an embedded UI for parameter tweaking (sliders, combos, buttons).
- Connections are drawn with Bézier curves for smooth visual flow.

## Design Rationale

- **Modularity**: Each operation is a node, making it easy to add or extend functionality.
- **Interactivity**: ImGui provides responsive UI elements with minimal setup.
- **Visualization**: Connections and previews make it intuitive to understand the data pipeline.
- **Separation of Concerns**: UI logic, processing logic, and graph management are decoupled.

## Libraries Used

- **OpenCV**: Image loading, saving, and processing.
- **ImGui**: Node interface, toolbox, properties panel, and interactive UI widgets.
- **GLFW**: Window/context creation.
- **GLEW**: OpenGL extension wrangler for compatibility.
- **tinyfiledialogs**: Simple platform-independent dialogs for file operations.

## Challenges Encountered

- Ensuring dynamic node spawning and connections without crashing.
- Handling multiple input types, especially in nodes like BlendNode with two inputs.
- Maintaining synchronization between UI state and graph logic.
- Fixing scroll and pan issues with the canvas.
- Avoiding visual glitches (e.g., overlapping widgets, incorrect texture previews).

## Improvements and Future Work

- Add node duplication and undo-redo system.
- Export/import node graphs as JSON.
- Support for more advanced filters and custom shaders.
- Allow adjustable Bézier curve styles and snapping grid.
