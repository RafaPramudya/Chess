#include "Chess.h"

#include <cmath>

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

			if (c == 'K')		whiteKingCastle = true;
			else if (c == 'Q')	whiteQueenCastle = true;
			else if (c == 'k')	blackKingCastle = true;
			else if (c == 'q')	blackQueenCastle = true;

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

	auto startPiece = piecesPlaced[move.startSquare.getIndex()];

	std::vector<Move> validMoves = Move::generateValidMoves(move.startSquare, this);
	bool valid = false;

	for (const auto& validMove : validMoves) {
		if (validMove.targetSquare == move.targetSquare) {
			valid = true;
			break;
		}
	}
	if (!valid) return;

	// EnPassant
	if ((startPiece.getId() % 8) == Piece::PAWN && move.targetSquare == getEnPassant()) {
		if (startPiece.isBlack())	piecesPlaced[(move.targetSquare + Move::UP).getIndex()] = Piece();
		else						piecesPlaced[(move.targetSquare + Move::DOWN).getIndex()] = Piece();
	}

	resetEnPassant();
	if ((startPiece.getId() % 8) == Piece::PAWN && std::abs(move.targetSquare.getRank() - move.startSquare.getRank()) == 2) {
		setEnPassant((startPiece.isBlack()) ? move.targetSquare + Move::UP : move.targetSquare + Move::DOWN);
		std::printf("En Passant Available");
	}

	// Castle
	if (move.targetSquare == Square("a1") || move.startSquare == Square("a1")) whiteQueenCastle = false;
	if (move.targetSquare == Square("h1") || move.startSquare == Square("h1")) whiteKingCastle = false;
	if (move.targetSquare == Square("a8") || move.startSquare == Square("a8")) blackQueenCastle = false;
	if (move.targetSquare == Square("h8") || move.startSquare == Square("h8")) blackKingCastle = false;
	if ((startPiece.getId() % 8) == Piece::KING) {
		if (startPiece.isBlack())	{ blackKingCastle = false; blackQueenCastle = false; }
		else						{ whiteKingCastle = false; whiteQueenCastle = false; }

		if (std::abs(move.startSquare.getFile() - move.targetSquare.getFile()) >= 2) {
			if (move.targetSquare.getFile() == 2) {
				piecesPlaced[(move.targetSquare + Move::RIGHT).getIndex()] = Piece(Piece::ROOK, startPiece.isWhite());
				piecesPlaced[(move.targetSquare + Move::LEFT * 2).getIndex()] = Piece();
			}
			else if (move.targetSquare.getFile() == 6) {
				piecesPlaced[(move.targetSquare + Move::LEFT).getIndex()] = Piece(Piece::ROOK, startPiece.isWhite());
				piecesPlaced[(move.targetSquare + Move::RIGHT).getIndex()] = Piece();
			}
		}
	}

	// Execute Move
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

std::vector<Move> Move::generateValidMoves(Square square, Board* board)
{
	assert((bool) square);
	Piece piece = board->getPiece(square);
	std::vector<Move> results;

	if (piece.isBlack() == board->whiteToMove) return results;
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
		if (!board->getPiece(targetSquare)) { 
			results.emplace_back(targetSquare);
			if (doubleSquare && !board->getPiece(doubleSquare)) { results.emplace_back(doubleSquare); }
		}

		// Capture
		if (board->getPiece(targetSquare + LEFT).isEnemy(piece) || (targetSquare + LEFT == board->getEnPassant())) results.emplace_back(targetSquare + LEFT);
		if (board->getPiece(targetSquare + RIGHT).isEnemy(piece) || (targetSquare + RIGHT == board->getEnPassant())) results.emplace_back(targetSquare + RIGHT);
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

			auto obstacle = board->getPiece(targetSquare);
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
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// UR
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP + RIGHT;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// DL
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN + LEFT;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// DR
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN + RIGHT;
			obstacle = board->getPiece(targetSquare);

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
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7) {
			targetSquare = targetSquare + RIGHT;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP;
			obstacle = board->getPiece(targetSquare);

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
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7) {
			targetSquare = targetSquare + RIGHT;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// UL
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP + LEFT;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// UR
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 7) {
			targetSquare = targetSquare + UP + RIGHT;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// DL
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN + LEFT;
			obstacle = board->getPiece(targetSquare);

			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// DR
		obstacle = Piece();
		targetSquare = square;
		while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 0) {
			targetSquare = targetSquare + DOWN + RIGHT;
			obstacle = board->getPiece(targetSquare);

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

			auto obstacle = board->getPiece(targetSquare);
			if (!obstacle || piece.isEnemy(obstacle)) results.emplace_back(targetSquare);
		}

		// Castle
		if (board->getWhiteKingCastle() && piece.isWhite()) {
			if (!board->getPiece(square + RIGHT) && !board->getPiece(square + RIGHT * 2)) results.emplace_back(square + RIGHT * 2);
		}
		if (board->getWhiteQueenCastle() && piece.isWhite()) {
			if (!board->getPiece(square + LEFT) && !board->getPiece(square + LEFT * 2) && !board->getPiece(square + LEFT * 3)) results.emplace_back(square + LEFT * 2);
		}
		if (board->getBlackKingCastle() && piece.isBlack()) {
			if (!board->getPiece(square + RIGHT) && !board->getPiece(square + RIGHT * 2)) results.emplace_back(square + RIGHT * 2);
		}
		if (board->getBlackQueenCastle() && piece.isBlack()) {
			if (!board->getPiece(square + LEFT) && !board->getPiece(square + LEFT * 2) && !board->getPiece(square + LEFT * 3)) results.emplace_back(square + LEFT * 2);
		}
	}
		break;
	default:
		break;
	}

	return results;
}
