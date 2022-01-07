/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Koen Samyn
/*=============================================================================*/
// FMatrix.cpp: FMatrix class
/*=============================================================================*/
#ifndef ELITE_MATH_FMATRIX
#define	ELITE_MATH_FMATRIX

#include <random>
namespace Elite 
{
	class FMatrix
	{
	private:
		float* m_Data;
		int m_Rows, m_Columns;
		int m_Size;
		int rcToIndex(int r, int c)
		{
			return c * m_Rows + r;
		}
	public:
		FMatrix(): m_Data(nullptr) {}
		FMatrix(int rows, int columns): 
			m_Rows(rows),
			m_Columns(columns),
			m_Data(new float[rows * columns]),
			m_Size(rows * columns)
		{}

		virtual ~FMatrix()
		{
			delete[] m_Data;
			m_Data = nullptr;
		}

		void Resize(int nrOfRows, int nrOfColumns)
		{
			m_Rows = nrOfRows;
			m_Columns = nrOfColumns;
			m_Size = m_Rows * m_Columns;
			m_Data = new float[m_Size];
		}

		void Set(int row, int column, float value)
		{
			int index = rcToIndex(row, column);
			if (index > -1 && index < m_Size) 
			{
				m_Data[index] = value;
			}
			else 
			{
				printf("Wrong index! [%d, %d]\n", row, column);
			}
		}
		void SetAll(float value)
		{
			for (int i = 0; i < m_Size; ++i)
			{
				m_Data[i] = value;
			}
		}
		void SetRowAll(int row, float value) 
		{
			for (int c = 0; c < m_Columns; ++c) 
			{
				Set(row, c, value);
			}
		}

		void Add(int row, int column, float toAdd)
		{
			int index = rcToIndex(row, column);
			if (index > -1 && index < m_Size) {
				m_Data[index] += toAdd;
			}
			else {
				printf("Wrong index! [%d, %d]\n", row, column);
			}
		}


		float Get(int row, int column)
		{
			int index = rcToIndex(row, column);
			if (index > -1 && index < m_Size) {
				return m_Data[index];
			}
			else {
				return -1;
			}
		}

		void Randomize(float min, float max)
		{
			for (int i = 0; i < m_Size; ++i)
			{
				float r = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
				m_Data[i] = r;
			}
		}

		int GetNrOfRows() 
		{
			return m_Rows;
		}
		int GetNrOfColumns() 
		{
			return m_Columns;
		}
		void MatrixMultiply(FMatrix& op2, FMatrix& result)
		{
			int maxRows = min(GetNrOfRows(), result.GetNrOfRows());
			int maxColumns = min(op2.GetNrOfColumns(), result.GetNrOfColumns());

			for (int c_row = 0; c_row < maxRows; ++c_row)
			{
				for (int c_column = 0; c_column < maxColumns; ++c_column)
				{
					float sum = 0;
					for (int index = 0; index < GetNrOfColumns(); ++index)
					{
						sum += Get(c_row, index) * op2.Get(index, c_column);
					}
					result.Set(c_row, c_column, sum);
				}
			}
		}
		void ScalarMultiply(float scalar)
		{
			for (int i = 0; i < m_Size; ++i) 
			{
				m_Data[i] *= scalar;
			}
		}

		void Copy(FMatrix& other)
		{
			int maxRows = min(GetNrOfRows(), other.GetNrOfRows());
			int maxColumns = min(GetNrOfColumns(), other.GetNrOfColumns());

			for (int c_row = 0; c_row < maxRows; ++c_row) {
				for (int c_column = 0; c_column < maxColumns; ++c_column) {
					float oVal = other.Get(c_row, c_column);
					Set(c_row, c_column, oVal);
				}
			}
		}

		void Add(FMatrix& other)
		{
			int maxRows = min(GetNrOfRows(), other.GetNrOfRows());
			int maxColumns = min(GetNrOfColumns(), other.GetNrOfColumns());

			for (int c_row = 0; c_row < maxRows; ++c_row) {
				for (int c_column = 0; c_column < maxColumns; ++c_column) {
					float oVal = other.Get(c_row, c_column);
					float thisVal = Get(c_row, c_column);
					Set(c_row, c_column, thisVal + oVal);
				}
			}
		}

		void Subtract(FMatrix& other)
		{
			int maxRows = min(GetNrOfRows(), other.GetNrOfRows());
			int maxColumns = min(GetNrOfColumns(), other.GetNrOfColumns());

			for (int c_row = 0; c_row < maxRows; ++c_row) 
			{
				for (int c_column = 0; c_column < maxColumns; ++c_column) 
				{
					float oVal = other.Get(c_row, c_column);
					float thisVal = Get(c_row, c_column);
					Set(c_row, c_column, thisVal - oVal);
				}
			}
		}
		void Sigmoid()
		{
			for (int i = 0; i < m_Size; ++i)
			{
				float val = m_Data[i];
				m_Data[i] = 1 / (1 + exp(-val));
			}
		}

		float Sum()
		{
			float sum = 0;
			for (int i = 0; i < m_Size; ++i)
			{
				sum += m_Data[i];
			}
			return sum;
		}
		float Dot(FMatrix& op2)
		{
			int mR = min(GetNrOfRows(), op2.GetNrOfRows());
			int mC = min(GetNrOfColumns(), op2.GetNrOfColumns());

			float dot = 0;
			for (int c_row = 0; c_row < mR; ++c_row) {
				for (int c_column = 0; c_column < mC; ++c_column) {
					float v1 = Get(c_row, c_column);
					float v2 = Get(c_row, c_column);
					dot += v1 * v2;
				}
			}
			return dot;
		}
		float Max()
		{
			float max = -FLT_MAX;
			for (int c_row = 0; c_row < m_Rows; ++c_row) {
				for (int c_column = 0; c_column < m_Columns; ++c_column) {
					float value = Get(c_row, c_column);
					if (value > max) {
						max = value;
					}

				}
			}
			return max;
		}
		float Max(int& r, int& c)
		{
			float max = -FLT_MAX;
			for (int c_row = 0; c_row < m_Rows; ++c_row) {
				for (int c_column = 0; c_column < m_Columns; ++c_column) {
					float value = Get(c_row, c_column);
					if (value > max) {
						max = value;
						r = c_row;
						c = c_column;
					}

				}
			}
			return max;
		}
		float MaxOfRow(int r)
		{
			float max = -FLT_MAX;
			for (int c_column = 0; c_column < m_Columns; ++c_column) {
				float value = Get(r, c_column);
				if (value > max) {
					max = value;

				}
			}
			return max;
		}


		void Print()
		{
			for (int c_row = 0; c_row < m_Rows; ++c_row) {
				for (int c_column = 0; c_column < m_Columns; ++c_column) {
					float value = Get(c_row, c_column);
					printf("%.3f\t", value);
				}
				printf("\n");
			}
		}
	};
}
#endif

