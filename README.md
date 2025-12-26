<div align="center">
    <img width="264" height="156" alt="appicon" src="https://github.com/user-attachments/assets/54eb478c-6f00-426d-adf3-40c58737fa93" />
    <br/>
    <h1>SimpleCards</h1>
    <h3>Minimal flashcards software for studying using spaced repetition technique.</h3>
</div>

# Install

<details>
<summary><strong>Linux</strong></summary><br>
    
AppImage is available [here](https://github.com/GioBigno/SimpleCards/releases)

</details>

<details>
<summary><strong>WebAssembly</strong></summary><br>

A demo is available [here](https://www.bignosoft.com/simplecards-demo/)

</details>

# Build from source

<details>
<summary><strong>Linux</strong></summary><br>
    
1. **Install build dependencies**

    Requires Qt 6.10
4. **Clone the repository**

    ```bash
    git clone https://github.com/GioBigno/simple-cards.git && cd simple-cards
    ```
5. **Compile**

    ```bash
    # Configure
    mkdir build && cd build
    cmake .. -DCMAKE_PREFIX_PATH=<path/to/Qt> -DCMAKE_BUILD_TYPE=Release
    # Build
    cmake --build .
    ```
6. **Install**
    ```bash
    cmake --install . --prefix <custom/installation/prefix>
    ```

</details>

<details>
<summary><strong>WebAssembly</strong></summary><br>
    
1. **Install build dependencies**

    Requires
   - Qt 6.10
   - [Emscripten](https://doc.qt.io/qt-6/wasm.html)
2. **Clone the repository**

    ```bash
    git clone https://github.com/GioBigno/simple-cards.git && cd simple-cards
    ```
3. **Compile**

    ```bash
    # Configure
    mkdir build && cd build
    /path/to/qt-wasm/qtbase/bin/qt-cmake .. -DCMAKE_PREFIX_PATH=<path/to/Qt> -DCMAKE_BUILD_TYPE=Release
    # Build
    cmake --build .
    ```
4. **Install**
   
    Setup an HTTP server in the build directory and access `simplecards.html`. <br>
    If using wasm_multithread (suggested) the server must send `Cross-Origin-Opener-Policy` and `Cross-Origin-Embedder-Policy` headers.

</details>

# Extra
icons: https://www.svgrepo.com/collection/chunk-16px-thick-interface-icons
