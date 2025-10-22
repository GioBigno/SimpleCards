<div align="center">
    <img width="364" height="256" alt="appicon" src="https://github.com/user-attachments/assets/54eb478c-6f00-426d-adf3-40c58737fa93" />
    <br/>
    <h1>SimpleCards</h1>
    <h3>Minimal flashcards software for studying using spaced repetition technique.</h3>
</div>

# Preview

# Install (Linux)

# Build from source (Linux)

1. **Install build dependencies** 

    Requires Qt 6.10

2. **Clone the repository**

    ```bash
    git clone https://github.com/GioBigno/simple-cards.git && cd simple-cards
    ```
3. **Compile**

    ```bash
    # Configure
    mkdir build && cd build
    cmake .. -DCMAKE_PREFIX_PATH=<path/to/Qt> -DCMAKE_BUILD_TYPE=Release
    # Build
    cmake --build .
    ```
4. **Install**
    ```bash
    cmake --install . --prefix <custom/installation/prefix>
    ```
