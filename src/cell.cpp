#include "cell.h"

std::unique_ptr<Cell> generateCell(Cell::Type t)
{
	switch (t)
	{
	case Cell::Type::dead:
		return std::make_unique<DeadCell>();
	case Cell::Type::alive:
		return std::make_unique<AliveCell>();
	default:
		return nullptr;
	}
}