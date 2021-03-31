#include "pipeline/pipeline.h"

//static variables definition
std::unique_ptr<Surface> pipeline::m_texture = nullptr;
Mat3 pipeline::m_rotationMatrix = Mat3::Identity();
Vec3 pipeline::m_translation = Vec3(0.0f, 0.0f, 0.0f);
Graphics* pipeline::gfx = nullptr;

std::vector<int> pipeline::vertexProcessed = std::vector<int>();