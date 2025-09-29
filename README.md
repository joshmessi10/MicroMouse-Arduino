# 🐭 Arduino Micromouse – Intelligent Agent Maze Solver (DFS + Path Optimization)

This project implements an **Intelligent Micromouse Agent** that can scan, map, and solve mazes.  
It integrates **Depth-First Search (DFS)** exploration, **string-based path storage**, and **path reduction (BFS-like optimization)**.  
Control and monitoring are done via a **Bluetooth mobile app**, making it interactive and easy to debug.  
It also includes a **Flood Fill framework** (not fully implemented yet), allowing future improvements for competition-level performance.

---

## 🤖 Introduction to AI and Intelligent Agents

**Artificial Intelligence (AI)** is the field of computer science dedicated to creating systems that can **perceive, reason, and act** to achieve goals.  
One of the key concepts in AI is the **Intelligent Agent**:

- **Agent**: An entity that perceives its environment through sensors and acts upon that environment through actuators.  
- **Rationality**: An intelligent agent chooses actions that maximize its chances of success based on its knowledge.  
- **Autonomy**: An agent learns from experience and makes decisions without human intervention.  

In this project, the **Micromouse Robot** is modeled as an intelligent agent:  

- **Perception** → Sensors detect walls and open paths.  
- **Decision-making** → DFS algorithm selects which path to explore.  
- **Action** → Motors execute forward, left, right, or back movements.  

This structure transforms the micromouse into a **goal-oriented agent**, whose objective is to reach the center of the maze.

---

## 🚀 Features

- 📱 **Bluetooth Control**: Start scanning, solving, and view live maze map as a **character grid**.  
- 🧭 **Defined Start Position**: Robot begins scanning from a fixed starting cell.  
- 🧮 **DFS Exploration**: Explores the maze by moving forward, left, right, or backtracking.
- ✅ **Maze Map Storage** in a `char[][]` grid updated in real time.   
- 📜 **Path Storage**: Saves moves as a string (`L`, `R`, `S`, `B`).  
- 🔄 **Path Reduction**: Optimizes stored path using replacement rules (BFS-style) to get shortest route to goal.
- ✅ **Orientation Tracking** (`N`, `E`, `S`, `W`) for accurate decision-making.  
- 🗺️ **Variable Goal**: Destination cell is predefined but flexible.  
- 💡 **LED Indicators**: Show the current action (turning, moving, backtracking).  
- ⚙️ **Calibrated Turns**: Uses **PWM + timing** to ensure precise 90° rotations despite friction.  
- 🔮 **Flood Fill (Future)**: Algorithm present in code but not fully implemented.  


---

## 🚀 Features

- ✅ **DFS Maze Solving Algorithm**: Recursive exploration of the maze.  
- ✅ **Autonomous Navigation**: Robot explores and backtracks when dead ends are found.  
- ✅ **Sensor Integration**: Detects walls to decide available moves.  
- ✅ **Motor Control**: Smooth forward/turn actions with PWM.  
- ✅ **Intelligent Agent Design**: Clear separation of perception, decision-making, and action.  

---

## 💻 Code Workflow

### **`MicroMouse.ino`**
- Initializes Arduino, Bluetooth, sensors, and motors.  
- Waits for Bluetooth commands to:  
  - Start scanning.  
  - Solve the maze.  
  - Show live character map of explored cells.  
- Executes the **DFS exploration loop**, sending updates back via Bluetooth.  

### **`functions.ino`**
- **Movement control**: Forward, left, right, back (calibrated with PWM + timing).  
- **DFS implementation**:  
  - Marks visited cells.  
  - Explores neighbors based on available walls.  
  - Stores sequence of moves in a string.  
- **Path optimization**:  
  - Applies replacement rules  to reduce path length.  
- **Orientation tracking**: Maintains current facing direction (`N`, `E`, `S`, `W`).  
- **LED feedback**: Indicates movement or decision-making actions.  


---

## 🧩 How It Works

1. User sends **Bluetooth command** to start scanning.  
2. Robot begins at a **defined starting cell** with a **predefined goal**.  
3. Using **DFS**, it explores the maze:  
   - Moves forward if path is clear.  
   - Turns left/right when necessary.  
   - Backtracks if dead end.  
   - Updates a `char` map of the labyrinth in real time.  
4. During exploration, it records turns (`L`, `R`, `S`, `B`) in a **path string**.  
5. Once the goal is reached, the path string is **simplified (string reduction rules)** into the shortest path.  
6. Robot can then replay the **optimized path** to reach the goal faster.  


---

## 🧠 Algorithm Summary

### **DFS Exploration**
1. Start at predefined cell.  
2. Sense walls and possible moves.  
3. Choose an unexplored direction → move.  
4. If dead end → backtrack.  
5. Log moves (`L`, `R`, `S`, `B`).  

### **Path Reduction (BFS-like Simplification)**
- After full exploration, stored path is optimized by applying replacement rules.  
- Example:  
  - Raw path: `FFLFRRBBFF`  
  - Optimized path: `FFRF` (shortest sequence to goal).  

### **Flood Fill (Planned)**
- Algorithm is coded but not active yet.  
- Will allow **fast goal-oriented solving** after initial exploration.  

---

## 🔧 Hardware Requirements

- Arduino Uno / Nano / Mega  
- Motor driver (L298N / TB6612FNG)  
- DC motors with encoders (recommended)  
- IR or ultrasonic sensors for wall detection  
- Bluetooth module (HC-05 / HC-06)  
- LEDs for status indication  
- Power supply (LiPo battery)  

---

## 🧠 Path Optimization (String Reduction)

After DFS exploration, the robot has a **raw path string** like:  

```SSLBRSBSLBR```

Using **replacement rules**, the path is simplified into a shorter, optimized path (similar to BFS shortest path), as it only follows branches that get to the goal. Each rule removes redundant detours caused by DFS backtracking:

- "LBR" → "B" : Going left, back, then right is equivalent to a single back.

- "LBS" → "R" : Left + back + straight simplifies to just turning right.

- "RBL" → "B" : Right + back + left is equivalent to going back.

- "SBL" → "R" : Straight + back + left reduces to right.

- "SBS" → "B" : Straight + back + straight equals going back.

- "LBL" → "S" : Left + back + left simplifies to going straight.

- "RBR" → "S" : Right + back + right also simplifies to straight.

This ensures the robot can solve the maze by following th path till the corresponding goal.

After applying rules:
```SSRBR```


Final optimized path:
```SSS```

---

## 📊 Future Improvements

- 🔄 Full **Flood Fill algorithm** for optimal solving.  
- 🧮 Use **BFS directly** for shortest path.  
- 🖥️ Develop a **graphical simulator** for testing DFS and path reduction.  
- 🧭 Add IMU/gyroscope for more accurate turning.

---

## 📸 Demo

![Micromouse](https://github.com/user-attachments/assets/6d924d4a-c0dc-48dd-a0f5-2cb37e757df0)

---

## 👨‍💻 Author

Developed by **Josh Sebastián López Murcia**  
