#include "RenderingContext.h"

// This constructor is done for you
inline RenderingContext::RenderingContext()
{
  depth=0;
  Matrix::SetIdentity(viewmatrix);
  Matrix::SetIdentity(projectionmatrix);
}  

inline void RenderingContext::InitModelMatrix(bool resetdepth)
{
  if (resetdepth)
    depth=0;
  Matrix::SetIdentity(modelmatrix + depth);
}

inline void RenderingContext::PushModelMatrix()
{
  memcpy(modelmatrix+depth+16, modelmatrix+depth, sizeof(float)*16);
  depth+=16;
}

inline void RenderingContext::PopModelMatrix()
{
  assert(depth>0);
  depth-=16;
}



// This method is done for you
inline void RenderingContext::UpdateMVPs()
{
  Matrix::MultiplyMatrix(mvmatrix, viewmatrix, modelmatrix+depth);
  Matrix::MultiplyMatrix(mvpmatrix, projectionmatrix, mvmatrix);
}

inline void RenderingContext::Translate(const float x, const float y, const float z)
{
  Matrix::Translate(modelmatrix+depth, x, y, z);
}

inline void RenderingContext::Scale(const float x, const float y, const float z)
{
  Matrix::Scale(modelmatrix+depth, x, y, z);
}

inline void RenderingContext::RotateX(const float degs)
{
  Matrix::RotateX(modelmatrix+depth, degs);
}

inline void RenderingContext::RotateY(const float degs)
{
  Matrix::RotateY(modelmatrix+depth, degs);
}

inline void RenderingContext::RotateZ(const float degs)
{
  Matrix::RotateZ(modelmatrix+depth, degs);
}
