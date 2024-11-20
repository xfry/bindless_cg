#include <iostream>
#include "viewport.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_obj_file>" << std::endl;
        return EXIT_FAILURE;
    }

    // Inicializamos el Viewport con el archivo OBJ proporcionado
    char* winTitle = "OpenGL Viewport";
    // Instanciamos el Viewport
    Viewport viewport(800, 600, winTitle, argv[1]);
    // Inicializamos el Viewport
    viewport.Init();
    // Corremos el Viewport
    viewport.Run();
    
    return 0;
}
