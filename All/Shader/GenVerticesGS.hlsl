cbuffer cbChangerEveryFrame : register(b0)
{
	matrix gWorld;
}


	static const uint	case_to_numpolys[256] = {
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 2, 1, 2, 2, 3, 2, 3, 3, 4,
		2, 3, 3, 4, 3, 4, 4, 3, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 3,
		2, 3, 3, 2, 3, 4, 4, 3, 3, 4, 4, 3, 4, 5, 5, 2, 1, 2, 2, 3, 2, 3, 3, 4,
		2, 3, 3, 4, 3, 4, 4, 3, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 4,
		2, 3, 3, 4, 3, 4, 2, 3, 3, 4, 4, 5, 4, 5, 3, 2, 3, 4, 4, 3, 4, 5, 3, 2,
		4, 5, 5, 4, 5, 2, 4, 1, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 3,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 2, 4, 3, 4, 3, 5, 2, 2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 4, 3, 4, 4, 3, 4, 5, 5, 4, 4, 3, 5, 2, 5, 4, 2, 1,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 2, 3, 3, 2, 3, 4, 4, 5, 4, 5, 5, 2,
		4, 3, 5, 4, 3, 2, 4, 1, 3, 4, 4, 5, 4, 5, 3, 4, 4, 5, 5, 2, 3, 4, 2, 1,
		2, 3, 3, 2, 3, 4, 2, 1, 3, 2, 4, 1, 2, 1, 1, 0
	};

	static const uint2	edge_to_verindexs[12] = {
		uint2(0, 1), uint2(1, 2), uint2(2, 3), uint2(3, 0), uint2(4, 5), uint2(5, 6),
		uint2(6, 7), uint2(7, 4), uint2(0, 4), uint2(1, 5), uint2(2, 6), uint2(3, 7)
	};

	static const int3	g_triTable[256][5] = { 
	{ int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 8, 3), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 1, 9), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(1, 8, 3), int3(9, 8, 1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 2, 10), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 8, 3), int3(1, 2, 10), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 2, 10), int3(0, 2, 9), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(2, 8, 3), int3(2, 10, 8), int3(10, 9, 8), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 11, 2), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(0, 11, 2), int3(8, 11, 0), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 9, 0), int3(2, 3, 11), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 11, 2), int3(1, 9, 11), int3(9, 8, 11), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(3, 10, 1), int3(11, 10, 3), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 10, 1), int3(0, 8, 10), int3(8, 11, 10), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 9, 0), int3(3, 11, 9), int3(11, 10, 9), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 8, 10), int3(10, 8, 11), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 7, 8), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 3, 0), int3(7, 3, 4), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(0, 1, 9), int3(8, 4, 7), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 1, 9), int3(4, 7, 1), int3(7, 3, 1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 2, 10), int3(8, 4, 7), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(3, 4, 7), int3(3, 0, 4), int3(1, 2, 10), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(9, 2, 10), int3(9, 0, 2), int3(8, 4, 7), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(2, 10, 9), int3(2, 9, 7), int3(2, 7, 3), int3(7, 9, 4), int3(-1, -1, -1) },
	{ int3(8, 4, 7), int3(3, 11, 2), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(11, 4, 7), int3(11, 2, 4), int3(2, 0, 4), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(9, 0, 1), int3(8, 4, 7), int3(2, 3, 11), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(4, 7, 11), int3(9, 4, 11), int3(9, 11, 2), int3(9, 2, 1), int3(-1, -1, -1) }, { int3(3, 10, 1), int3(3, 11, 10), int3(7, 8, 4), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 11, 10), int3(1, 4, 11), int3(1, 0, 4), int3(7, 11, 4), int3(-1, -1, -1) },
	{ int3(4, 7, 8), int3(9, 0, 11), int3(9, 11, 10), int3(11, 0, 3), int3(-1, -1, -1) }, { int3(4, 7, 11), int3(4, 11, 9), int3(9, 11, 10), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(9, 5, 4), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 5, 4), int3(0, 8, 3), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 5, 4), int3(1, 5, 0), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(8, 5, 4), int3(8, 3, 5), int3(3, 1, 5), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(1, 2, 10), int3(9, 5, 4), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 0, 8), int3(1, 2, 10), int3(4, 9, 5), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(5, 2, 10), int3(5, 4, 2), int3(4, 0, 2), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(2, 10, 5), int3(3, 2, 5), int3(3, 5, 4), int3(3, 4, 8), int3(-1, -1, -1) }, { int3(9, 5, 4), int3(2, 3, 11), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 11, 2), int3(0, 8, 11), int3(4, 9, 5), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(0, 5, 4), int3(0, 1, 5), int3(2, 3, 11), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(2, 1, 5), int3(2, 5, 8), int3(2, 8, 11), int3(4, 8, 5), int3(-1, -1, -1) }, { int3(10, 3, 11), int3(10, 1, 3), int3(9, 5, 4), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(4, 9, 5), int3(0, 8, 1), int3(8, 10, 1), int3(8, 11, 10), int3(-1, -1, -1) }, { int3(5, 4, 0), int3(5, 0, 11), int3(5, 11, 10), int3(11, 0, 3), int3(-1, -1, -1) }, { int3(5, 4, 8), int3(5, 8, 10), int3(10, 8, 11), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 7, 8), int3(5, 7, 9), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(9, 3, 0), int3(9, 5, 3), int3(5, 7, 3), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 7, 8), int3(0, 1, 7), int3(1, 5, 7), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(1, 5, 3), int3(3, 5, 7), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(9, 7, 8), int3(9, 5, 7), int3(10, 1, 2), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(10, 1, 2), int3(9, 5, 0), int3(5, 3, 0), int3(5, 7, 3), int3(-1, -1, -1) },
	{ int3(8, 0, 2), int3(8, 2, 5), int3(8, 5, 7), int3(10, 5, 2), int3(-1, -1, -1) }, { int3(2, 10, 5), int3(2, 5, 3), int3(3, 5, 7), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(7, 9, 5), int3(7, 8, 9), int3(3, 11, 2), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 5, 7), int3(9, 7, 2), int3(9, 2, 0), int3(2, 7, 11), int3(-1, -1, -1) }, { int3(2, 3, 11), int3(0, 1, 8), int3(1, 7, 8), int3(1, 5, 7), int3(-1, -1, -1) }, { int3(11, 2, 1), int3(11, 1, 7), int3(7, 1, 5), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 5, 8), int3(8, 5, 7), int3(10, 1, 3), int3(10, 3, 11), int3(-1, -1, -1) }, { int3(5, 7, 0), int3(5, 0, 9), int3(7, 11, 0), int3(1, 0, 10), int3(11, 10, 0) }, { int3(11, 10, 0), int3(11, 0, 3), int3(10, 5, 0), int3(8, 0, 7), int3(5, 7, 0) },
	{ int3(11, 10, 5), int3(7, 11, 5), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(10, 6, 5), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 8, 3), int3(5, 10, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 0, 1), int3(5, 10, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 8, 3), int3(1, 9, 8), int3(5, 10, 6), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 6, 5), int3(2, 6, 1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(1, 6, 5), int3(1, 2, 6), int3(3, 0, 8), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(9, 6, 5), int3(9, 0, 6), int3(0, 2, 6), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(5, 9, 8), int3(5, 8, 2), int3(5, 2, 6), int3(3, 2, 8), int3(-1, -1, -1) },
	{ int3(2, 3, 11), int3(10, 6, 5), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(11, 0, 8), int3(11, 2, 0), int3(10, 6, 5), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 1, 9), int3(2, 3, 11), int3(5, 10, 6), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(5, 10, 6), int3(1, 9, 2), int3(9, 11, 2), int3(9, 8, 11), int3(-1, -1, -1) }, { int3(6, 3, 11), int3(6, 5, 3), int3(5, 1, 3), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 8, 11), int3(0, 11, 5), int3(0, 5, 1), int3(5, 11, 6), int3(-1, -1, -1) },
	{ int3(3, 11, 6), int3(0, 3, 6), int3(0, 6, 5), int3(0, 5, 9), int3(-1, -1, -1) }, { int3(6, 5, 9), int3(6, 9, 11), int3(11, 9, 8), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(5, 10, 6), int3(4, 7, 8), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(4, 3, 0), int3(4, 7, 3), int3(6, 5, 10), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 9, 0), int3(5, 10, 6), int3(8, 4, 7), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(10, 6, 5), int3(1, 9, 7), int3(1, 7, 3), int3(7, 9, 4), int3(-1, -1, -1) },
	{ int3(6, 1, 2), int3(6, 5, 1), int3(4, 7, 8), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 2, 5), int3(5, 2, 6), int3(3, 0, 4), int3(3, 4, 7), int3(-1, -1, -1) }, { int3(8, 4, 7), int3(9, 0, 5), int3(0, 6, 5), int3(0, 2, 6), int3(-1, -1, -1) },
	{ int3(7, 3, 9), int3(7, 9, 4), int3(3, 2, 9), int3(5, 9, 6), int3(2, 6, 9) }, { int3(3, 11, 2), int3(7, 8, 4), int3(10, 6, 5), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(5, 10, 6), int3(4, 7, 2), int3(4, 2, 0), int3(2, 7, 11), int3(-1, -1, -1) },
	{ int3(0, 1, 9), int3(4, 7, 8), int3(2, 3, 11), int3(5, 10, 6), int3(-1, -1, -1) }, { int3(9, 2, 1), int3(9, 11, 2), int3(9, 4, 11), int3(7, 11, 4), int3(5, 10, 6) }, { int3(8, 4, 7), int3(3, 11, 5), int3(3, 5, 1), int3(5, 11, 6), int3(-1, -1, -1) },
	{ int3(5, 1, 11), int3(5, 11, 6), int3(1, 0, 11), int3(7, 11, 4), int3(0, 4, 11) }, { int3(0, 5, 9), int3(0, 6, 5), int3(0, 3, 6), int3(11, 6, 3), int3(8, 4, 7) }, { int3(6, 5, 9), int3(6, 9, 11), int3(4, 7, 9), int3(7, 11, 9), int3(-1, -1, -1) },
	{ int3(10, 4, 9), int3(6, 4, 10), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 10, 6), int3(4, 9, 10), int3(0, 8, 3), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(10, 0, 1), int3(10, 6, 0), int3(6, 4, 0), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(8, 3, 1), int3(8, 1, 6), int3(8, 6, 4), int3(6, 1, 10), int3(-1, -1, -1) }, { int3(1, 4, 9), int3(1, 2, 4), int3(2, 6, 4), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 0, 8), int3(1, 2, 9), int3(2, 4, 9), int3(2, 6, 4), int3(-1, -1, -1) },
	{ int3(0, 2, 4), int3(4, 2, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(8, 3, 2), int3(8, 2, 4), int3(4, 2, 6), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(10, 4, 9), int3(10, 6, 4), int3(11, 2, 3), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(0, 8, 2), int3(2, 8, 11), int3(4, 9, 10), int3(4, 10, 6), int3(-1, -1, -1) }, { int3(3, 11, 2), int3(0, 1, 6), int3(0, 6, 4), int3(6, 1, 10), int3(-1, -1, -1) }, { int3(6, 4, 1), int3(6, 1, 10), int3(4, 8, 1), int3(2, 1, 11), int3(8, 11, 1) },
	{ int3(9, 6, 4), int3(9, 3, 6), int3(9, 1, 3), int3(11, 6, 3), int3(-1, -1, -1) }, { int3(8, 11, 1), int3(8, 1, 0), int3(11, 6, 1), int3(9, 1, 4), int3(6, 4, 1) }, { int3(3, 11, 6), int3(3, 6, 0), int3(0, 6, 4), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(6, 4, 8), int3(11, 6, 8), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(7, 10, 6), int3(7, 8, 10), int3(8, 9, 10), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 7, 3), int3(0, 10, 7), int3(0, 9, 10), int3(6, 7, 10), int3(-1, -1, -1) },
	{ int3(10, 6, 7), int3(1, 10, 7), int3(1, 7, 8), int3(1, 8, 0), int3(-1, -1, -1) }, { int3(10, 6, 7), int3(10, 7, 1), int3(1, 7, 3), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 2, 6), int3(1, 6, 8), int3(1, 8, 9), int3(8, 6, 7), int3(-1, -1, -1) },
	{ int3(2, 6, 9), int3(2, 9, 1), int3(6, 7, 9), int3(0, 9, 3), int3(7, 3, 9) }, { int3(7, 8, 0), int3(7, 0, 6), int3(6, 0, 2), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(7, 3, 2), int3(6, 7, 2), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(2, 3, 11), int3(10, 6, 8), int3(10, 8, 9), int3(8, 6, 7), int3(-1, -1, -1) }, { int3(2, 0, 7), int3(2, 7, 11), int3(0, 9, 7), int3(6, 7, 10), int3(9, 10, 7) }, { int3(1, 8, 0), int3(1, 7, 8), int3(1, 10, 7), int3(6, 7, 10), int3(2, 3, 11) },
	{ int3(11, 2, 1), int3(11, 1, 7), int3(10, 6, 1), int3(6, 7, 1), int3(-1, -1, -1) }, { int3(8, 9, 6), int3(8, 6, 7), int3(9, 1, 6), int3(11, 6, 3), int3(1, 3, 6) }, { int3(0, 9, 1), int3(11, 6, 7), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(7, 8, 0), int3(7, 0, 6), int3(3, 11, 0), int3(11, 6, 0), int3(-1, -1, -1) }, { int3(7, 11, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(7, 6, 11), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(3, 0, 8), int3(11, 7, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 1, 9), int3(11, 7, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(8, 1, 9), int3(8, 3, 1), int3(11, 7, 6), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(10, 1, 2), int3(6, 11, 7), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 2, 10), int3(3, 0, 8), int3(6, 11, 7), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(2, 9, 0), int3(2, 10, 9), int3(6, 11, 7), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(6, 11, 7), int3(2, 10, 3), int3(10, 8, 3), int3(10, 9, 8), int3(-1, -1, -1) }, { int3(7, 2, 3), int3(6, 2, 7), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(7, 0, 8), int3(7, 6, 0), int3(6, 2, 0), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(2, 7, 6), int3(2, 3, 7), int3(0, 1, 9), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 6, 2), int3(1, 8, 6), int3(1, 9, 8), int3(8, 7, 6), int3(-1, -1, -1) }, { int3(10, 7, 6), int3(10, 1, 7), int3(1, 3, 7), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(10, 7, 6), int3(1, 7, 10), int3(1, 8, 7), int3(1, 0, 8), int3(-1, -1, -1) }, { int3(0, 3, 7), int3(0, 7, 10), int3(0, 10, 9), int3(6, 10, 7), int3(-1, -1, -1) }, { int3(7, 6, 10), int3(7, 10, 8), int3(8, 10, 9), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(6, 8, 4), int3(11, 8, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 6, 11), int3(3, 0, 6), int3(0, 4, 6), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(8, 6, 11), int3(8, 4, 6), int3(9, 0, 1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 4, 6), int3(9, 6, 3), int3(9, 3, 1), int3(11, 3, 6), int3(-1, -1, -1) }, { int3(6, 8, 4), int3(6, 11, 8), int3(2, 10, 1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 2, 10), int3(3, 0, 11), int3(0, 6, 11), int3(0, 4, 6), int3(-1, -1, -1) },
	{ int3(4, 11, 8), int3(4, 6, 11), int3(0, 2, 9), int3(2, 10, 9), int3(-1, -1, -1) }, { int3(10, 9, 3), int3(10, 3, 2), int3(9, 4, 3), int3(11, 3, 6), int3(4, 6, 3) }, { int3(8, 2, 3), int3(8, 4, 2), int3(4, 6, 2), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(0, 4, 2), int3(4, 6, 2), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 9, 0), int3(2, 3, 4), int3(2, 4, 6), int3(4, 3, 8), int3(-1, -1, -1) }, { int3(1, 9, 4), int3(1, 4, 2), int3(2, 4, 6), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(8, 1, 3), int3(8, 6, 1), int3(8, 4, 6), int3(6, 10, 1), int3(-1, -1, -1) }, { int3(10, 1, 0), int3(10, 0, 6), int3(6, 0, 4), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 6, 3), int3(4, 3, 8), int3(6, 10, 3), int3(0, 3, 9), int3(10, 9, 3) },
	{ int3(10, 9, 4), int3(6, 10, 4), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 9, 5), int3(7, 6, 11), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 8, 3), int3(4, 9, 5), int3(11, 7, 6), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(5, 0, 1), int3(5, 4, 0), int3(7, 6, 11), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(11, 7, 6), int3(8, 3, 4), int3(3, 5, 4), int3(3, 1, 5), int3(-1, -1, -1) }, { int3(9, 5, 4), int3(10, 1, 2), int3(7, 6, 11), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(6, 11, 7), int3(1, 2, 10), int3(0, 8, 3), int3(4, 9, 5), int3(-1, -1, -1) }, { int3(7, 6, 11), int3(5, 4, 10), int3(4, 2, 10), int3(4, 0, 2), int3(-1, -1, -1) }, { int3(3, 4, 8), int3(3, 5, 4), int3(3, 2, 5), int3(10, 5, 2), int3(11, 7, 6) },
	{ int3(7, 2, 3), int3(7, 6, 2), int3(5, 4, 9), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(9, 5, 4), int3(0, 8, 6), int3(0, 6, 2), int3(6, 8, 7), int3(-1, -1, -1) }, { int3(3, 6, 2), int3(3, 7, 6), int3(1, 5, 0), int3(5, 4, 0), int3(-1, -1, -1) },
	{ int3(6, 2, 8), int3(6, 8, 7), int3(2, 1, 8), int3(4, 8, 5), int3(1, 5, 8) }, { int3(9, 5, 4), int3(10, 1, 6), int3(1, 7, 6), int3(1, 3, 7), int3(-1, -1, -1) }, { int3(1, 6, 10), int3(1, 7, 6), int3(1, 0, 7), int3(8, 7, 0), int3(9, 5, 4) },
	{ int3(4, 0, 10), int3(4, 10, 5), int3(0, 3, 10), int3(6, 10, 7), int3(3, 7, 10) }, { int3(7, 6, 10), int3(7, 10, 8), int3(5, 4, 10), int3(4, 8, 10), int3(-1, -1, -1) }, { int3(6, 9, 5), int3(6, 11, 9), int3(11, 8, 9), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(3, 6, 11), int3(0, 6, 3), int3(0, 5, 6), int3(0, 9, 5), int3(-1, -1, -1) }, { int3(0, 11, 8), int3(0, 5, 11), int3(0, 1, 5), int3(5, 6, 11), int3(-1, -1, -1) }, { int3(6, 11, 3), int3(6, 3, 5), int3(5, 3, 1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(1, 2, 10), int3(9, 5, 11), int3(9, 11, 8), int3(11, 5, 6), int3(-1, -1, -1) }, { int3(0, 11, 3), int3(0, 6, 11), int3(0, 9, 6), int3(5, 6, 9), int3(1, 2, 10) }, { int3(11, 8, 5), int3(11, 5, 6), int3(8, 0, 5), int3(10, 5, 2), int3(0, 2, 5) },
	{ int3(6, 11, 3), int3(6, 3, 5), int3(2, 10, 3), int3(10, 5, 3), int3(-1, -1, -1) }, { int3(5, 8, 9), int3(5, 2, 8), int3(5, 6, 2), int3(3, 8, 2), int3(-1, -1, -1) }, { int3(9, 5, 6), int3(9, 6, 0), int3(0, 6, 2), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(1, 5, 8), int3(1, 8, 0), int3(5, 6, 8), int3(3, 8, 2), int3(6, 2, 8) }, { int3(1, 5, 6), int3(2, 1, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 3, 6), int3(1, 6, 10), int3(3, 8, 6), int3(5, 6, 9), int3(8, 9, 6) },
	{ int3(10, 1, 0), int3(10, 0, 6), int3(9, 5, 0), int3(5, 6, 0), int3(-1, -1, -1) }, { int3(0, 3, 8), int3(5, 6, 10), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(10, 5, 6), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(11, 5, 10), int3(7, 5, 11), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(11, 5, 10), int3(11, 7, 5), int3(8, 3, 0), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(5, 11, 7), int3(5, 10, 11), int3(1, 9, 0), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(10, 7, 5), int3(10, 11, 7), int3(9, 8, 1), int3(8, 3, 1), int3(-1, -1, -1) }, { int3(11, 1, 2), int3(11, 7, 1), int3(7, 5, 1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 8, 3), int3(1, 2, 7), int3(1, 7, 5), int3(7, 2, 11), int3(-1, -1, -1) },
	{ int3(9, 7, 5), int3(9, 2, 7), int3(9, 0, 2), int3(2, 11, 7), int3(-1, -1, -1) }, { int3(7, 5, 2), int3(7, 2, 11), int3(5, 9, 2), int3(3, 2, 8), int3(9, 8, 2) }, { int3(2, 5, 10), int3(2, 3, 5), int3(3, 7, 5), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(8, 2, 0), int3(8, 5, 2), int3(8, 7, 5), int3(10, 2, 5), int3(-1, -1, -1) }, { int3(9, 0, 1), int3(5, 10, 3), int3(5, 3, 7), int3(3, 10, 2), int3(-1, -1, -1) }, { int3(9, 8, 2), int3(9, 2, 1), int3(8, 7, 2), int3(10, 2, 5), int3(7, 5, 2) },
	{ int3(1, 3, 5), int3(3, 7, 5), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 8, 7), int3(0, 7, 1), int3(1, 7, 5), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(9, 0, 3), int3(9, 3, 5), int3(5, 3, 7), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 8, 7), int3(5, 9, 7), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(5, 8, 4), int3(5, 10, 8), int3(10, 11, 8), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(5, 0, 4), int3(5, 11, 0), int3(5, 10, 11), int3(11, 3, 0), int3(-1, -1, -1) },
	{ int3(0, 1, 9), int3(8, 4, 10), int3(8, 10, 11), int3(10, 4, 5), int3(-1, -1, -1) }, { int3(10, 11, 4), int3(10, 4, 5), int3(11, 3, 4), int3(9, 4, 1), int3(3, 1, 4) }, { int3(2, 5, 1), int3(2, 8, 5), int3(2, 11, 8), int3(4, 5, 8), int3(-1, -1, -1) },
	{ int3(0, 4, 11), int3(0, 11, 3), int3(4, 5, 11), int3(2, 11, 1), int3(5, 1, 11) }, { int3(0, 2, 5), int3(0, 5, 9), int3(2, 11, 5), int3(4, 5, 8), int3(11, 8, 5) }, { int3(9, 4, 5), int3(2, 11, 3), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(2, 5, 10), int3(3, 5, 2), int3(3, 4, 5), int3(3, 8, 4), int3(-1, -1, -1) }, { int3(5, 10, 2), int3(5, 2, 4), int3(4, 2, 0), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 10, 2), int3(3, 5, 10), int3(3, 8, 5), int3(4, 5, 8), int3(0, 1, 9) },
	{ int3(5, 10, 2), int3(5, 2, 4), int3(1, 9, 2), int3(9, 4, 2), int3(-1, -1, -1) }, { int3(8, 4, 5), int3(8, 5, 3), int3(3, 5, 1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 4, 5), int3(1, 0, 5), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(8, 4, 5), int3(8, 5, 3), int3(9, 0, 5), int3(0, 3, 5), int3(-1, -1, -1) }, { int3(9, 4, 5), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 11, 7), int3(4, 9, 11), int3(9, 10, 11), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(0, 8, 3), int3(4, 9, 7), int3(9, 11, 7), int3(9, 10, 11), int3(-1, -1, -1) }, { int3(1, 10, 11), int3(1, 11, 4), int3(1, 4, 0), int3(7, 4, 11), int3(-1, -1, -1) }, { int3(3, 1, 4), int3(3, 4, 8), int3(1, 10, 4), int3(7, 4, 11), int3(10, 11, 4) },
	{ int3(4, 11, 7), int3(9, 11, 4), int3(9, 2, 11), int3(9, 1, 2), int3(-1, -1, -1) }, { int3(9, 7, 4), int3(9, 11, 7), int3(9, 1, 11), int3(2, 11, 1), int3(0, 8, 3) }, { int3(11, 7, 4), int3(11, 4, 2), int3(2, 4, 0), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(11, 7, 4), int3(11, 4, 2), int3(8, 3, 4), int3(3, 2, 4), int3(-1, -1, -1) }, { int3(2, 9, 10), int3(2, 7, 9), int3(2, 3, 7), int3(7, 4, 9), int3(-1, -1, -1) }, { int3(9, 10, 7), int3(9, 7, 4), int3(10, 2, 7), int3(8, 7, 0), int3(2, 0, 7) },
	{ int3(3, 7, 10), int3(3, 10, 2), int3(7, 4, 10), int3(1, 10, 0), int3(4, 0, 10) }, { int3(1, 10, 2), int3(8, 7, 4), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 9, 1), int3(4, 1, 7), int3(7, 1, 3), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(4, 9, 1), int3(4, 1, 7), int3(0, 8, 1), int3(8, 7, 1), int3(-1, -1, -1) }, { int3(4, 0, 3), int3(7, 4, 3), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(4, 8, 7), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 10, 8), int3(10, 11, 8), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 0, 9), int3(3, 9, 11), int3(11, 9, 10), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 1, 10), int3(0, 10, 8), int3(8, 10, 11), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(3, 1, 10), int3(11, 3, 10), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(1, 2, 11), int3(1, 11, 9), int3(9, 11, 8), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 0, 9), int3(3, 9, 11), int3(1, 2, 9), int3(2, 11, 9), int3(-1, -1, -1) },
	{ int3(0, 2, 11), int3(8, 0, 11), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(3, 2, 11), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(2, 3, 8), int3(2, 8, 10), int3(10, 8, 9), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(9, 10, 2), int3(0, 9, 2), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(2, 3, 8), int3(2, 8, 10), int3(0, 1, 8), int3(1, 10, 8), int3(-1, -1, -1) }, { int3(1, 10, 2), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(1, 3, 8), int3(9, 1, 8), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 9, 1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) }, { int3(0, 3, 8), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) },
	{ int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1), int3(-1, -1, -1) } };

	static const float3 VoxelPoints[8] = { float3(0, 0, 0), float3(0, 0.03125f, 0), float3(0.03125f, 0.03125f, 0), float3(0.03125f, 0, 0),
		float3(0, 0, 0.03125f), float3(0, 0.03125f, 0.03125f), float3(0.03125f, 0.03125f, 0.03125f),
		float3(0.03125f, 0, 0.03125f) };

struct VertexOut
{
	float3 PosL : POSITION;
	float Density[8] : PSIZE;
	uint Case : POSITION1;
};

struct GeoOut
{
	float3 PosW : POSITION;
};

float3 CalcLerp(VertexOut geoin[1], uint2 ver)
{
	float3 texcoord[3];
	float t = -geoin[0].Density[ver.x] / (geoin[0].Density[ver.y] - geoin[0].Density[ver.x]);
	float3 s = float3(t, t, t);
	texcoord[0] = geoin[0].PosL + VoxelPoints[ver.x];
	texcoord[1] = geoin[0].PosL + VoxelPoints[ver.y];
	texcoord[2] = lerp(texcoord[0], texcoord[1], s);
	return mul(float4(texcoord[2], 1.0f), gWorld).xyz;
}

#define DEBUGSO 0
#if DEBUGSO
[maxvertexcount(3)]
#else
[maxvertexcount(15)]
#endif
void main(
	point VertexOut geoin[1],
	inout TriangleStream< GeoOut> tristream
)
{
	uint nums = case_to_numpolys[geoin[0].Case];
	uint CaseIndex = geoin[0].Case;
	int3 tri;
	uint2 ver;
	float t;
	float3 s;
	float3 texcoord[3];
	GeoOut gout;
#if DEBUGSO
	gout.PosW = float3(CaseIndex, geoin[0].Density[0], geoin[0].Density[1]);
	tristream.Append(gout);
	gout.PosW = float3(geoin[0].Density[2], geoin[0].Density[3], geoin[0].Density[4]);
	tristream.Append(gout);
	gout.PosW = float3(geoin[0].Density[5], geoin[0].Density[6], geoin[0].Density[7]);
	tristream.Append(gout);
	tristream.RestartStrip();
#else
	
	for (uint i = 0; i < nums; ++i)
	{
		tri = g_triTable[CaseIndex][i];
		ver = edge_to_verindexs[tri.x];
		gout.PosW = CalcLerp(geoin,ver);
		tristream.Append(gout);
		
		tri = g_triTable[CaseIndex][i];
		ver = edge_to_verindexs[tri.y];
		gout.PosW = CalcLerp(geoin, ver);
		tristream.Append(gout);

		tri = g_triTable[CaseIndex][i];
		ver = edge_to_verindexs[tri.z];
		gout.PosW = CalcLerp(geoin, ver);
		tristream.Append(gout);

		tristream.RestartStrip();
	}
#endif
	/**/
}