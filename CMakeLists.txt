cmake_minimum_required(VERSION 3.30)
project(P2_ABlanco_PMendez_FPicado)

set(CMAKE_CXX_STANDARD 20)

add_executable(P2_ABlanco_PMendez_FPicado main.cpp
        P2_ABlanco_PMendez_FPicado/Tower.cpp
        P2_ABlanco_PMendez_FPicado/Tower.h
        P2_ABlanco_PMendez_FPicado/Enemy.cpp
        P2_ABlanco_PMendez_FPicado/Enemy.h
        P2_ABlanco_PMendez_FPicado/Node.cpp
        P2_ABlanco_PMendez_FPicado/Node.h
        P2_ABlanco_PMendez_FPicado/Map.cpp
        P2_ABlanco_PMendez_FPicado/Map.h
        P2_ABlanco_PMendez_FPicado/Main.cpp
        P2_ABlanco_PMendez_FPicado/GameWindow.cpp
        P2_ABlanco_PMendez_FPicado/GameWindow.h
        P2_ABlanco_PMendez_FPicado/EnemyUnit.cpp
        P2_ABlanco_PMendez_FPicado/EnemyUnit.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.6.1/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

target_link_libraries(P2_ABlanco_PMendez_FPicado sfml-system sfml-window sfml-graphics sfml-audio)

