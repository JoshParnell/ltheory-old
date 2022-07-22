#if 0
      int sign(float x) const {
        return x > 0 ? 1 : 0;
      }

        output = Mesh_Create();
        Bound3 b = parent->p->bound;
        Array3DFloat const& grid = parent->grid;

        Array3D<bool> signGrid(grid.resX - 1, grid.resY - 1, grid.resZ - 1);
        Array3D<V3> gradGrid(grid.resX, grid.resY, grid.resZ);
        for (size_t x = 0; x < grid.resX; ++x)
        for (size_t y = 0; y < grid.resY; ++y)
        for (size_t z = 0; z < grid.resZ; ++z)
          gradGrid.SetData(V3(0.00001f, 0, 0), x, y, z);

        V3 cellSize = b.upper - b.lower;

        for (size_t x = 0; x < grid.resX - 1; ++x)
        for (size_t y = 0; y < grid.resY - 1; ++y)
        for (size_t z = 0; z < grid.resZ - 1; ++z) {
          float e000 = grid.GetData(x + 0, y + 0, z + 0);
          float e001 = grid.GetData(x + 0, y + 0, z + 1);
          float e010 = grid.GetData(x + 0, y + 1, z + 0);
          float e011 = grid.GetData(x + 0, y + 1, z + 1);
          float e100 = grid.GetData(x + 1, y + 0, z + 0);
          float e101 = grid.GetData(x + 1, y + 0, z + 1);
          float e110 = grid.GetData(x + 1, y + 1, z + 0);
          float e111 = grid.GetData(x + 1, y + 1, z + 1);

          /* Check for crossing. */
          int s =  sign(e000);
          if (s != sign(e001) ||
              s != sign(e010) ||
              s != sign(e011) ||
              s != sign(e100) ||
              s != sign(e101) ||
              s != sign(e110) ||
              s != sign(e111))
            signGrid.SetData(true, x, y, z);
          else
            signGrid.SetData(false, x, y, z);

          if (x && y && z) {
            V3 grad = Normalize(
              V3(e100 - grid.GetData(x - 1, y + 0, z + 0),
                 e010 - grid.GetData(x + 0, y - 1, z + 0),
                 e001 - grid.GetData(x + 0, y + 0, z - 1)));
            gradGrid.SetData(grad, x, y, z);
          }
        }

        for (size_t x = 0; x < signGrid.resX; ++x) {
          float x0 = (float)(x + 0) / (float)(grid.resX);
          float x1 = (float)(x + 1) / (float)(grid.resX);
          x0 = Mix(b.lower.x, b.upper.x, x0);
          x1 = Mix(b.lower.x, b.upper.x, x1);

          for (size_t y = 0; y < signGrid.resY; ++y) {
            float y0 = (float)(y + 0) / (float)(grid.resY);
            float y1 = (float)(y + 1) / (float)(grid.resY);
            y0 = Mix(b.lower.y, b.upper.y, y0);
            y1 = Mix(b.lower.y, b.upper.y, y1);

            for (size_t z = 0; z < signGrid.resZ; ++z) {
              float z0 = (float)(z + 0) / (float)(grid.resZ);
              float z1 = (float)(z + 1) / (float)(grid.resZ);
              z0 = Mix(b.lower.z, b.upper.z, z0);
              z1 = Mix(b.lower.z, b.upper.z, z1);

              bool self = signGrid.GetData(x, y, z);

              if (self) {
                bool xn = x > 0 ? signGrid.GetData(x - 1, y, z) : false;
                bool xp = x + 1 < signGrid.resX ? signGrid.GetData(x + 1, y, z) : false;
                bool yn = y > 0 ? signGrid.GetData(x, y - 1, z) : false;
                bool yp = y + 1 < signGrid.resY ? signGrid.GetData(x, y + 1, z) : false;
                bool zn = z > 0 ? signGrid.GetData(x, y, z - 1) : false;
                bool zp = z + 1 < signGrid.resZ ? signGrid.GetData(x, y, z + 1) : false;

                size_t off = output->vertices.size();

                const V3 k = V3(0.5f);

                /* 0 : + + + */ {
                  V3 n = gradGrid.GetData(x + 1, y + 1, z + 1);
                  float d = grid.GetData(x + 1, y + 1, z + 1);
                  output->AddVertex(V3(x1, y1, z1) - k * d * n, n, 1, 0);
                }

                /* 1 : - + + */ {
                  V3 n = gradGrid.GetData(x + 0, y + 1, z + 1);
                  float d = grid.GetData(x + 0, y + 1, z + 1);
                  output->AddVertex(V3(x0, y1, z1) - k * d * n, n, 0, 0);
                }
                
                /* 2 : - - + */ {
                  V3 n = gradGrid.GetData(x + 0, y + 0, z + 1);
                  float d = grid.GetData(x + 0, y + 0, z + 1);
                  output->AddVertex(V3(x0, y0, z1) - k * d * n, n, 0, 0);
                }
                
                /* 3 : + - + */ {
                  V3 n = gradGrid.GetData(x + 1, y + 0, z + 1);
                  float d = grid.GetData(x + 1, y + 0, z + 1);
                  output->AddVertex(V3(x1, y0, z1) - k * d * n, n, 0, 0);
                }
                
                /* 4 : + + - */ {
                  V3 n = gradGrid.GetData(x + 1, y + 1, z + 0);
                  float d = grid.GetData(x + 1, y + 1, z + 0);
                  output->AddVertex(V3(x1, y1, z0) - k * d * n, n, 0, 0);
                }
                
                /* 5 : - + - */ {
                  V3 n = gradGrid.GetData(x + 0, y + 1, z + 0);
                  float d = grid.GetData(x + 0, y + 1, z + 0);
                  output->AddVertex(V3(x0, y1, z0) - k * d * n, n, 0, 0);
                }
                
                /* 6 : - - - */ {
                  V3 n = gradGrid.GetData(x + 0, y + 0, z + 0);
                  float d = grid.GetData(x + 0, y + 0, z + 0);
                  output->AddVertex(V3(x0, y0, z0) - k * d * n, n, 0, 0);
                }
                
                /* 7 : + - - */ {
                  V3 n = gradGrid.GetData(x + 1, y + 0, z + 0);
                  float d = grid.GetData(x + 1, y + 0, z + 0);
                  output->AddVertex(V3(x1, y0, z0) - k * d * n, n, 0, 0);
                }

                /* Positive Z. */
                if (!zp) output->AddQuad(off + 0, off + 3, off + 2, off + 1);

                /* Negative Z. */
                if (!zn) output->AddQuad(off + 7, off + 4, off + 5, off + 6);

                /* Positive Y. */
                if (!yp) output->AddQuad(off + 0, off + 1, off + 5, off + 4);

                /* Negative Y. */
                if (!yn) output->AddQuad(off + 2, off + 3, off + 7, off + 6);

                /* Positive X. */
                if (!xp) output->AddQuad(off + 3, off + 0, off + 4, off + 7);

                /* Negative X. */
                if (!xn) output->AddQuad(off + 1, off + 2, off + 6, off + 5);
              }
            }
          }
        }
#endif
