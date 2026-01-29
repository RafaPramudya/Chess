#include "Chess.h"

Piece::Piece(uint8_t id)
{
	this->id = id;
}

Piece::Piece(Piece::Type type, bool isWhite)
{
	this->id = isWhite ? type : type + Type::BLACK;
}

Board::Board()
{
	for (int i = 0; i < 64; i++) piecesPlaced[i] = Piece(Piece::NONE);
}

Board::Board(Piece* pieceArray)
{
	std::memcpy(piecesPlaced, pieceArray, sizeof(Piece) * 64);
}

Board::Board(const char* fenNotation)
{
	size_t length = std::strlen(fenNotation);
	int8_t rankIndex = 7;
	int8_t fileIndex = 0;

	bool moveFlag = false;
	bool halfMove = true;

	for (int i = 0; i < length; i++) {
		char c = fenNotation[i];

		if (rankIndex >= 0) {
			if (c == '/') { fileIndex = 0;  rankIndex--; continue; }
			else if (rankIndex == 0 && fileIndex == 8) { rankIndex--; continue; }

			if (c <= '8' && c > '0') { fileIndex += c - '0'; assert(fileIndex >= 8);}
			else if (c == 'p' || c == 'P') { piecesPlaced[rankIndex * 8 + fileIndex] = Piece(Piece::PAWN,	static_cast<bool>(c - 'p')); fileIndex++; }
			else if (c == 'n' || c == 'N') { piecesPlaced[rankIndex * 8 + fileIndex] = Piece(Piece::KNIGHT, static_cast<bool>(c - 'n')); fileIndex++; }
			else if (c == 'b' || c == 'B') { piecesPlaced[rankIndex * 8 + fileIndex] = Piece(Piece::BISHOP, static_cast<bool>(c - 'b')); fileIndex++; }
			else if (c == 'r' || c == 'R') { piecesPlaced[rankIndex * 8 + fileIndex] = Piece(Piece::ROOK,	static_cast<bool>(c - 'r')); fileIndex++; }
			else if (c == 'q' || c == 'Q') { piecesPlaced[rankIndex * 8 + fileIndex] = Piece(Piece::QUEEN,	static_cast<bool>(c - 'q')); fileIndex++; }
			else if (c == 'k' || c == 'K') { piecesPlaced[rankIndex * 8 + fileIndex] = Piece(Piece::KING,	static_cast<bool>(c - 'k')); fileIndex++; }
		}
		else {
			if (c == ' ' || c == '-') continue;

			if (c == 'b' && !moveFlag)		{ whiteToMove = false; moveFlag = true; }
			else if (c == 'w' && !moveFlag)	{ whiteToMove = true; moveFlag = true; }

			if (c == 'K')		castleAvailable.whiteKing = true;
			else if (c == 'Q')	castleAvailable.whiteQueen = true;
			else if (c == 'k')	castleAvailable.blackKing = true;
			else if (c == 'q')	castleAvailable.blackQueen = true;

			if (c >= 'a' && c <= 'h') {
				char c_next = fenNotation[++i];
				char buffer[3] = { c, c_next, '\0' };

				enPassantSquare = Square(buffer);
			}

			if (c >= '0' && c <= '9') {
				while (c != ' ' && c != '\0') {
					if (halfMove) {
						halfMove *= 10;
						halfMove += (c - '0');
					}
					else {
						fullMove *= 10;
						fullMove += (c - '0');
					}
					c = fenNotation[++i];
				}

				halfMove = !halfMove;
			}
		}
	}
}

void Board::makeMove(Move& move)
{
	if (!move.startSquare || !move.targetSquare) throw std::exception("Error Square Invalid Value");

	auto& startPiece = piecesPlaced[move.startSquare.getIndex()];

	std::vector<Move> validMoves = Move::generateValidMoves(move.startSquare, getPieces());
	bool valid = false;

	for (const auto& validMove : validMoves) {
		if (validMove.targetSquare == move.targetSquare) {
			valid = true;
			break;
		}
	}
	if (!valid) return;

	piecesPlaced[move.targetSquare.getIndex()] = startPiece;
	piecesPlaced[move.startSquare.getIndex()] = Piece();

	whiteToMove = !whiteToMove;
}

void Board::printBoard(bool whiteSide)
{
	std::printf("Board :\n");
	if (whiteSide) {
		for (int file = 0; file < 8; file++) {
			std::printf("%c ", 'A' + file);
		}
		std::printf("\n-----------------\n");
		for (int rank = 7; rank >= 0; rank--) {
			for (int file = 0; file < 8; file++) {
				std::printf("%c ", piecesPlaced[rank * 8 + file].getNotation());
			}
			std::printf("| %d\n", rank + 1);
		}
	}
	else {
		for (int file = 0; file < 8; file++) {
			std::printf("%c ", 'H' - file);
		}
		std::printf("\n-----------------\n");
		for (int rank = 0; rank < 8; rank++) {
			for (int file = 7; file >= 0; file--) {
				std::printf("%c ", piecesPlaced[rank * 8 + file].getNotation());
			}
			std::printf("| %d\n", rank + 1);
		}
	}
}

std::vector<Move> Move::generateValidMoves(Square square, Piece* piecesArray)
{
	assert((bool) square);
	Piece& piece = piecesArray[square.getIndex()];
	std::vector<Move> results;

	switch ((piece.getId() % 8)) {
	case Piece::PAWN:
	{
		Square targetSquare;
		Square doubleSquare;

		if (piece.isWhite()) {
			targetSquare = square + UP;

			if ((square.getIndex() / 8) == 1)
				doubleSquare = square + 2 * UP;
		}
		else {
			targetSquare = square + DOWN;

			if ((square.getIndex() / 8) == 6)
				doubleSquare = square + 2 * DOWN;
		}

		// Move
		if (!encounterObstacle(targetSquare, piecesArray)) { 
			results.emplace_back(targetSquare);
			if (doubleSquare && !encounterObstacle(doubleSquare, piecesArray)) results.emplace_back(doubleSquare);
		}

		// Capture
		if (encounterObstacle(targetSquare + LEFT, piecesArray).isEnemy(piece)) results.emplace_back(targetSquare + LEFT);
		if (encounterObstacle(targetSquare + RIGHT, piecesArray).isEnemy(piece)) results.emplace_back(targetSquare + RIGHT);
	}
		break;
	case Piece::KNIGHT:
	{
		Square moveSquare[8] = {
			square + 2 * LEFT + UP, square + 2 * LEFT + DOWN,
			square + 2 * RIGHT + UP, square + 2 * RIGHT + DOWN,
			square + 2 * UP + LEFT, square + 2 * UP + RIGHT,
			square + 2 * DOWN + LEFT, square + 2 * DOWN + RIGHT,
		};

		for (int i = 0; i < 8; i++) {
			Square targetSquare = moveSquare[i];

			// Exceptions
			if (!targetSquare) continue;
			if (square.getFile() <= 1 && targetSquare.getFile() >= 6) continue;
			if (square.getFile() >= 6 && targetSquare.getFile() <= 1) continue;

			auto obstacle = encounterObstacle(targetSquare, piecesArray);
			if (!obstacle) results.emplace_back(targetSquare); // Move
			else if (piece.isEnemy(obstacle)) results.emplace_back(targetSquare); // Capture
		}
	}
		break;
	case Piece::BISHOP:
	{
		// UL
		Piece obstacle;
		Square targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP + LEFT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// UR
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP + RIGHT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// DL
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN + LEFT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// DR
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN + RIGHT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}
	}
		break;
	case Piece::ROOK:
	{
		Piece obstacle;
		Square targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0) {
			targetSquare = targetSquare + LEFT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7) {
			targetSquare = targetSquare + RIGHT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}
	}
		break;
	case Piece::QUEEN:
	{
		Piece obstacle;
		Square targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0) {
			targetSquare = targetSquare + LEFT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7) {
			targetSquare = targetSquare + RIGHT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// UL
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP + LEFT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// UR
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP + RIGHT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// DL
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN + LEFT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// DR
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN + RIGHT;
			obstacle = encounterObstacle(targetSquare, piecesArray);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}
	}
		break;
	case Piece::KING:
	{
		Square movingSquare[8] = {
			square + LEFT, square + RIGHT,
			square + UP, square + DOWN,
			square + LEFT + UP, square + RIGHT + UP,
			square + LEFT + DOWN, square + RIGHT + DOWN,
		};

		for (int i = 0; i < 8; i++) {
			Square targetSquare = movingSquare[i];
			if (square.getFile() == 0 && targetSquare.getFile() == 7) continue;
			if (square.getFile() == 7 && targetSquare.getFile() == 0) continue;

			auto obstacle = encounterObstacle(targetSquare, piecesArray);
			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}
	}
		break;
	default:
		break;
	}

	return results;
}
