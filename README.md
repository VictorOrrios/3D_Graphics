# G3D - Prácticas de OpenGL

Prácticas de la asignatura Gráficos 3D (G3D), Máster en Informática Gráfica, Juegos y Realidad
Virtual.

| Proyecto | Práctica |
| --- | --- |
| `p1GLSL` | 1. Introducción a GLSL |
| `p2GLSL` | 2. Iluminación y materiales en GLSL |
| `p3OGL` | 3. Introducción a la programación en OpenGL |
| `p4OGL` | 4. Aumento de realismo - Postproceso (MRT) |

## Dependencias

**No hay que instalar ninguna librería.** GLFW, GLAD, GLM y stb_image están dentro de la carpeta
`external/` y se compilan junto con el proyecto.

| Librería | Versión | Para qué se usa |
| --- | --- | --- |
| [GLFW](https://www.glfw.org/) | 3.4 | ventana, contexto de OpenGL y eventos |
| [GLAD](https://gen.glad.sh/) | 2.0.8 | carga de las funciones de OpenGL 4.6 core |
| [GLM](https://github.com/g-truc/glm) | 1.0.3 | matemáticas |
| [stb_image](https://github.com/nothings/stb) | - | carga de imágenes |

La aplicación pide un contexto de OpenGL 4.6 core y, si la tarjeta gráfica no llega, reintenta con
4.3 y después con 3.3, así que también funciona en equipos antiguos.

---

## Windows con Visual Studio

### Requisitos

Instala [Visual Studio 2022 Community](https://visualstudio.microsoft.com/es/vs/community/) y, en el
instalador, marca la carga de trabajo **Desarrollo para el escritorio con C++**. Esa carga de
trabajo ya incluye CMake, así que **no tienes que instalar nada más**.

### Pasos

1. Descomprime el fichero `g3d-exercises.zip` donde quieras trabajar.
2. Abre **Visual Studio 2022**.
3. En la ventana de inicio, elige **Abrir una carpeta local**.
   (Si Visual Studio ya está abierto: menú **Archivo > Abrir > Carpeta...**).
4. Selecciona la carpeta `g3d-exercises`, que es la que contiene el fichero
   `CMakeLists.txt`. **No abras una subcarpeta como `p1GLSL`.**
5. Espera a que Visual Studio configure el proyecto. Verás el progreso en la ventana **Salida**
   eligiendo *Mostrar salida de: CMake*. Termina cuando aparece
   `Generación de la caché de CMake finalizada`. La primera vez tarda un poco, porque compila GLFW.
6. En la barra de herramientas superior, despliega la lista de **elemento de inicio** (el botón con
   el triángulo verde) y elige la práctica que quieras ejecutar: `p1GLSL.exe`, `p2GLSL.exe`,
   `p3OGL.exe` o `p4OGL.exe`.
7. Pulsa **F5** para compilar y ejecutar con depurador, o **Ctrl+F5** para ejecutar sin él.
8. Para cambiar de práctica, vuelve al desplegable del paso 6 y elige otra.

### Si prefieres trabajar con una solución (.sln)

Abre el *Símbolo del sistema para desarrolladores de VS 2022* en la carpeta del proyecto y ejecuta:

```powershell
cmake --preset windows
cmake --build --preset windows
```

Se genera `build/g3d-practicas.sln`, que puedes abrir con Visual Studio. Los ejecutables quedan en
`build\p1GLSL\Debug\p1GLSL.exe` y equivalentes.

---

## Linux con VS Code

### Requisitos

Instala el compilador, CMake y las cabeceras de X11 que GLFW necesita para compilarse:

```bash
sudo apt install build-essential cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

En VS Code, instala estas dos extensiones:

- **C/C++** (`ms-vscode.cpptools`)
- **CMake Tools** (`ms-vscode.cmake-tools`)

### Pasos

1. Descomprime el fichero `g3d-exercises.zip` donde quieras trabajar.
2. Abre la carpeta en VS Code, desde el menú **File > Open Folder...** o desde la terminal:

   ```bash
   cd g3d-exercises
   code .
   ```

   Abre la carpeta que contiene el fichero `CMakeLists.txt`, no una subcarpeta.
3. CMake Tools detecta el proyecto y pregunta por el *preset* de configuración: elige **linux**.
   Si no te pregunta, pulsa `Ctrl+Shift+P` y ejecuta **CMake: Select Configure Preset**.
4. Abre el panel **Run and Debug** pulsando `Ctrl+Shift+D`. En la parte superior de ese panel hay
   un **desplegable** con el nombre de la configuración activa.
5. Despliégalo y elige la práctica que quieras: `Linux: p1GLSL`, `Linux: p2GLSL`, `Linux: p3OGL`
   o `Linux: p4OGL`.
6. Pulsa **F5**. Compila y arranca. La primera vez tarda un poco, porque compila GLFW.
7. Para cambiar de práctica, vuelve al desplegable del paso 5 y elige otra. No hace falta compilar
   a mano en ningún momento: **F5 compila siempre las cuatro prácticas antes de lanzar**.

> **Ojo:** la extensión CMake Tools tiene *su propio* selector de destino, en la barra de estado
> inferior, independiente de ese desplegable. Si ejecutas con el botón ▷ de esa barra en lugar de
> con F5, se compila y se lanza solo la práctica elegida ahí, que puede no ser la del panel Run and
> Debug. Con F5 no tienes ese problema, así que usa F5.

### Compilar y ejecutar desde la terminal

```bash
cmake --preset linux
cmake --build --preset linux

./build/p1GLSL/p1GLSL
./build/p2GLSL/p2GLSL
./build/p3OGL/p3OGL
./build/p4OGL/p4OGL
```

---

## Notas

- Los shaders y las texturas se localizan mediante rutas absolutas que define CMake
  (`SHADERS_PATH` y `TEXTURES_PATH`), así que da igual desde qué carpeta ejecutes el programa.
- Los errores de OpenGL se imprimen por consola gracias al *debug output*, disponible cuando el
  contexto obtenido es 4.3 o superior.
- Al arrancar, cada práctica imprime por consola la versión de OpenGL que ha conseguido, por
  ejemplo: `This system supports OpenGL Version: 4.6.0 NVIDIA 580.173.02`.

## Problemas frecuentes

| Síntoma | Causa y solución |
| --- | --- |
| Visual Studio no configura nada al abrir la carpeta | Has abierto una subcarpeta. Cierra y abre la carpeta que contiene `CMakeLists.txt`. |
| `p1GLSL` abre una ventana en negro | Es lo esperado: el shader de partida (`shader.v0`) está vacío, lo completas tú durante la práctica. |
| En Linux, error de CMake sobre `X11` o `Xrandr` | Faltan las cabeceras de X11: ejecuta el `sudo apt install` de la sección de requisitos. |
| `could not create the window ... OpenGL 3.3 core` | La tarjeta gráfica o el driver no llegan a OpenGL 3.3. Actualiza el driver. |
| En VS Code, `launch: program '.../build/p2GLSL/p2GLSL' does not exist` | Esa práctica no está compilada, porque se lanzó con el botón ▷ de la barra inferior, que solo compila el destino seleccionado ahí. Lánzala con **F5** desde el panel Run and Debug: compila las cuatro antes de arrancar. |
