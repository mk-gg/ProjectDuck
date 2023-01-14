#include "GameRenderer.h"
#include "Offset.h"
#include <cstring>

void GameRenderer::ReadFromBaseAddress(int baseAddr) 
{

	memcpy(&viewMatrix, (void*)(baseAddr + Offset::ViewMatrix), 16 * sizeof(float));
	memcpy(&projMatrix, (void*)(baseAddr + Offset::ProjectionMatrix), 16 * sizeof(float));

	char* addrRenderer = (char*)*(int*)(baseAddr + Offset::Renderer);
	memcpy(&width, addrRenderer + Offset::RendererWidth, sizeof(int));
	memcpy(&height, addrRenderer + Offset::RendererHeight, sizeof(int));

	MultiplyMatrices(viewProjMatrix, viewMatrix, 4, 4, projMatrix, 4, 4);
}

void GameRenderer::MultiplyMatrices(float* out, float* a, int row1, int col1, float* b, int row2, int col2)
{
	int size = row1 * col2;
	for (int i = 0; i < row1; i++) {
		for (int j = 0; j < col2; j++) {
			float sum = 0.f;
			for (int k = 0; k < col1; k++)
				sum = sum + a[i * col1 + k] * b[k * col2 + j];
			out[i * col2 + j] = sum;
		}
	}
}