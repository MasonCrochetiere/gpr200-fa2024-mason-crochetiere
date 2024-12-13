file(
 GLOB_RECURSE CORE_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)

set(CORE_INC_DIR
 ${CMAKE_CURRENT_SOURCE_DIR}
 CACHE PATH "CORE INCLUDE SOURCE PATH"
)

add_library(core STATIC ${CORE_SRC} ${CORE_INC} "macroLib/shader.h" "macroLib/shader.cpp" "macroLib/texture2D.h" "macroLib/camera.cpp" "macroLib/camera.h"
        MeshSystem/mesh.cpp
        MeshSystem/mesh.h
        MeshSystem/meshGenerator.cpp
        MeshSystem/meshGenerator.h
        MeshSystem/MeshRenderer.cpp
        MeshSystem/MeshRenderer.h "ParticleSystem/MemoryPool.cpp" "ParticleSystem/MemoryPool.h" "ParticleSystem/ParticleSystem.h" "ParticleSystem/ParticleSystem.cpp" "ParticleSystem/Particle.cpp" "ParticleSystem/Particle.h" "ParticleSystem/ParticleSystemValues.h" "ParticleSystem/ParticleSystemValues.cpp"
        Lighting/LightingSystem.cpp
        Lighting/LightingSystem.h "MeshSystem/Model.h" "MeshSystem/Model.cpp" "macroLib/shaderSystem.h" "macroLib/shaderSystem.cpp"   )

find_package(OpenGL REQUIRED)

target_link_libraries(core PUBLIC IMGUI glm assimp)

install (TARGETS core DESTINATION lib)
install (FILES ${CORE_INC} DESTINATION include/core)

