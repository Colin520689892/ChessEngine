#include "ChessEngine.h"

int main() {
	ChessEngine::beforeStart();
	ChessEngine::dian();
	while (ChessEngine::isSomeOneWin() == -1) {
		int x, y;
		std::cin >> x >> y;
		ChessEngine::nextStep(x, y);
		Position p = ChessEngine::getLastPosition();
		std::cout << p.x << " " << p.y << "\n";
		ChessEngine::print();
	}
	std::cout << ChessEngine::isSomeOneWin() << "\n";
	return 0;
}