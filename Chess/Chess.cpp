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

bool Board::isSquareAttacked(Square sq, bool attackerIsWhite)
{
	Square knightMoveSquare[8] = {
		sq + 2 * Move::LEFT + Move::UP,		sq + 2 * Move::LEFT + Move::DOWN,
		sq + 2 * Move::RIGHT + Move::UP,	sq + 2 * Move::RIGHT + Move::DOWN,
		sq + 2 * Move::UP + Move::LEFT,		sq + 2 * Move::UP + Move::RIGHT,
		sq + 2 * Move::DOWN + Move::LEFT,	sq + 2 * Move::DOWN + Move::RIGHT,
	};

	// LEFT
	Piece obstacle;
	Square targetSquare = sq;
	while (!obstacle && targetSquare.getFile() != 0) {
		targetSquare = targetSquare + Move::LEFT;
		obstacle = getPiece(targetSquare);

		if (obstacle) {
			if ((obstacle.getType() == Piece::ROOK || obstacle.getType() == Piece::QUEEN) && (obstacle.isWhite() == attackerIsWhite)) { return true; }
			break;
		}
	}
	// RIGHT
	obstacle = Piece();
	targetSquare = sq;
	while (!obstacle && targetSquare.getFile() != 7) {
		targetSquare = targetSquare + Move::RIGHT;
		obstacle = getPiece(targetSquare);

		if (obstacle) {
			if ((obstacle.getType() == Piece::ROOK || obstacle.getType() == Piece::QUEEN) && (obstacle.isWhite() == attackerIsWhite)) { return true; }
			break;
		}
	}
	// UP
	obstacle = Piece();
	targetSquare = sq;
	while (!obstacle && targetSquare.getRank() != 7) {
		targetSquare = targetSquare + Move::UP;
		obstacle = getPiece(targetSquare);

		if (obstacle) {
			if ((obstacle.getType() == Piece::ROOK || obstacle.getType() == Piece::QUEEN) && (obstacle.isWhite() == attackerIsWhite)) { return true; }
			break;
		}
	}
	// DOWN
	obstacle = Piece();
	targetSquare = sq;
	while (!obstacle && targetSquare.getRank() != 0) {
		targetSquare = targetSquare + Move::DOWN;
		obstacle = getPiece(targetSquare);

		if (obstacle) {
			if ((obstacle.getType() == Piece::ROOK || obstacle.getType() == Piece::QUEEN) && (obstacle.isWhite() == attackerIsWhite)) { return true; }
			break;
		}
	}
	// UR
	obstacle = Piece();
	targetSquare = sq;
	while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 7) {
		targetSquare = targetSquare + Move::UP + Move::RIGHT;
		obstacle = getPiece(targetSquare);

		if (obstacle) {
			if ((obstacle.getType() == Piece::BISHOP || obstacle.getType() == Piece::QUEEN) && (obstacle.isWhite() == attackerIsWhite)) { return true; }
			break;
		}
	}
	// UL
	obstacle = Piece();
	targetSquare = sq;
	while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 7) {
		targetSquare = targetSquare + Move::UP + Move::LEFT;
		obstacle = getPiece(targetSquare);

		if (obstacle) {
			if ((obstacle.getType() == Piece::BISHOP || obstacle.getType() == Piece::QUEEN) && (obstacle.isWhite() == attackerIsWhite)) { return true; }
			break;
		}
	}
	// DL
	obstacle = Piece();
	targetSquare = sq;
	while (!obstacle && targetSquare.getFile() != 0 && targetSquare.getRank() != 0) {
		targetSquare = targetSquare + Move::DOWN + Move::LEFT;
		obstacle = getPiece(targetSquare);

		if (obstacle) {
			if ((obstacle.getType() == Piece::BISHOP || obstacle.getType() == Piece::QUEEN) && (obstacle.isWhite() == attackerIsWhite)) { return true; }
			break;
		}
	}
	// DR
	obstacle = Piece();
	targetSquare = sq;
	while (!obstacle && targetSquare.getFile() != 7 && targetSquare.getRank() != 0) {
		targetSquare = targetSquare + Move::DOWN + Move::RIGHT;
		obstacle = getPiece(targetSquare);

		if (obstacle) {
			if ((obstacle.getType() == Piece::BISHOP || obstacle.getType() == Piece::QUEEN) && (obstacle.isWhite() == attackerIsWhite)) { return true; }
			break;
		}
	}
	// Knight
	for (int i = 0; i < 8; i++) {
		Square targetSquare = knightMoveSquare[i];

		// Exceptions
		if (!targetSquare) continue;
		if (sq.getFile() <= 1 && targetSquare.getFile() >= 6) continue;
		if (sq.getFile() >= 6 && targetSquare.getFile() <= 1) continue;

		auto obstacle = getPiece(targetSquare);
		if (obstacle.getType() == Piece::KNIGHT && (obstacle.isWhite() == attackerIsWhite)) { return true; }
	}
	// Pawn
	if (!attackerIsWhite) {
		Piece ulPiece = getPiece(sq + Move::UP + Move::LEFT);
		Piece urPiece = getPiece(sq + Move::UP + Move::RIGHT);

		if (sq.getFile() != 0 && ulPiece.getType() == Piece::PAWN && (ulPiece.isWhite() == attackerIsWhite)) { return true; }
		if (sq.getFile() != 7 && urPiece.getType() == Piece::PAWN && (urPiece.isWhite() == attackerIsWhite)) { return true; }
	}
	else {
		Piece dlPiece = getPiece(sq + Move::DOWN + Move::LEFT);
		Piece drPiece = getPiece(sq + Move::DOWN + Move::RIGHT);

		if (sq.getFile() != 0 && dlPiece.getType() == Piece::PAWN && (dlPiece.isWhite() == attackerIsWhite)) { return true; }
		if (sq.getFile() != 7 && drPiece.getType() == Piece::PAWN && (drPiece.isWhite() == attackerIsWhite)) { return true; }
	}

	return false;
}

void Board::analyzeCheck()
{
	whiteKingChecked = isSquareAttacked(whiteKingSquare, false);
	blackKingChecked = isSquareAttacked(blackKingSquare, true);
}

bool Board::analyzeCheck(bool white)
{
	return isSquareAttacked((white) ? whiteKingSquare : blackKingSquare, false);
}

Board::Board()
{
	for (int i = 0; i < 64; i++) piecesPlaced[i] = Piece(Piece::NONE);

	analyzeCheck();
}

Board::Board(Piece* pieceArray)
{
	std::memcpy(piecesPlaced, pieceArray, sizeof(Piece) * 64);
	int square = 0;
	for (; square < 64; square++) {
		Piece piece = piecesPlaced[square];
		if (piece && piece.getType() == Piece::KING) {
			if (piece.isWhite())	whiteKingSquare = Square(square);
			else					blackKingSquare = Square(square);
		}
	}

	analyzeCheck();
}

Board::Board(const char* fenNotation)
{
	size_t length = std::strlen(fenNotation);
	int8_t rankIndex = 7;
	int8_t fileIndex = 0;

	bool moveFlag = false;
	bool halfMoveFlag = true;

	for (int i = 0; i < length; i++) {
		char c = fenNotation[i];

		if (rankIndex >= 0) {
			if (c == '/') { fileIndex = 0;  rankIndex--; continue; }
			else if (rankIndex == 0 && fileIndex >= 8) { rankIndex--; continue; }

			uint8_t squareIndex = rankIndex * 8 + fileIndex;

			if (c <= '8' && c > '0') { fileIndex += c - '0'; }
			else if (c == 'p' || c == 'P') { piecesPlaced[squareIndex] = Piece(Piece::PAWN,		static_cast<bool>(c - 'p')); fileIndex++; }
			else if (c == 'n' || c == 'N') { piecesPlaced[squareIndex] = Piece(Piece::KNIGHT,	static_cast<bool>(c - 'n')); fileIndex++; }
			else if (c == 'b' || c == 'B') { piecesPlaced[squareIndex] = Piece(Piece::BISHOP,	static_cast<bool>(c - 'b')); fileIndex++; }
			else if (c == 'r' || c == 'R') { piecesPlaced[squareIndex] = Piece(Piece::ROOK,		static_cast<bool>(c - 'r')); fileIndex++; }
			else if (c == 'q' || c == 'Q') { piecesPlaced[squareIndex] = Piece(Piece::QUEEN,	static_cast<bool>(c - 'q')); fileIndex++; }
			else if (c == 'k' || c == 'K') { piecesPlaced[squareIndex] = Piece(Piece::KING,		static_cast<bool>(c - 'k')); fileIndex++;
				if (c - 'k') {
					whiteKingSquare = Square(squareIndex);
				}
				else {
					blackKingSquare = Square(squareIndex);
				}
			}
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
					if (halfMoveFlag) {
						halfMove *= 10;
						halfMove += (c - '0');
					}
					else {
						fullMove *= 10;
						fullMove += (c - '0');
					}
					c = fenNotation[++i];
				}

				halfMoveFlag = !halfMoveFlag;
			}
		}
	}

	analyzeCheck();
}

void Board::makeMove(Move& move)
{
	/*if (!move.startSquare || !move.targetSquare) throw std::exception("Error Square Invalid Value");*/

	auto startPiece = piecesPlaced[move.startSquare.getIndex()];

	// Update King Position
	if (startPiece.getType() == Piece::KING) {
		if (startPiece.isWhite())	whiteKingSquare = move.targetSquare;
		else						blackKingSquare = move.targetSquare;
	}

	// EnPassant
	if ((startPiece.getType()) == Piece::PAWN && move.targetSquare == getEnPassant()) {
		if (startPiece.isBlack())	piecesPlaced[(move.targetSquare + Move::UP).getIndex()] = Piece();
		else						piecesPlaced[(move.targetSquare + Move::DOWN).getIndex()] = Piece();

		move.enPassant = true;
	}

	resetEnPassant();
	if ((startPiece.getType()) == Piece::PAWN && std::abs(move.targetSquare.getRank() - move.startSquare.getRank()) == 2) {
		setEnPassant((startPiece.isBlack()) ? move.targetSquare + Move::UP : move.targetSquare + Move::DOWN);
	}

	// Castle
	if (move.targetSquare == Square("a1") || move.startSquare == Square("a1")) whiteQueenCastle = false;
	if (move.targetSquare == Square("h1") || move.startSquare == Square("h1")) whiteKingCastle = false;
	if (move.targetSquare == Square("a8") || move.startSquare == Square("a8")) blackQueenCastle = false;
	if (move.targetSquare == Square("h8") || move.startSquare == Square("h8")) blackKingCastle = false;
	if ((startPiece.getType()) == Piece::KING) {
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

			move.castle = true;
		}
	}

	// Execute Move
	move.capturedPiece = piecesPlaced[move.targetSquare.getIndex()];

	piecesPlaced[move.targetSquare.getIndex()] = startPiece;
	piecesPlaced[move.startSquare.getIndex()] = Piece();

	// Special Move Flag
	move.lastBoardWhiteCheck = whiteKingChecked;
	move.lastBoardBlackCheck = blackKingChecked;

	analyzeCheck();

	whiteToMove = !whiteToMove;
}

void Board::unMakeMove(Move move)
{
	if (!move.startSquare || !move.targetSquare) return;

	auto movingPiece = piecesPlaced[move.targetSquare.getIndex()];

	// Update King Posititon
	if (movingPiece.getType() == Piece::KING) {
		if (movingPiece.isWhite())	whiteKingSquare = move.startSquare;
		else						blackKingSquare = move.startSquare;
	}

	piecesPlaced[move.startSquare.getIndex()] = movingPiece;
	piecesPlaced[move.targetSquare.getIndex()] = move.capturedPiece;

	// EnPassant
	if (move.enPassant) {
		if (move.targetSquare.getRank() == 5) {
			piecesPlaced[(move.targetSquare + Move::DOWN).getIndex()] = Piece(Piece::PAWN, false);
		}
		else if (move.targetSquare.getRank() == 2) {
			piecesPlaced[(move.targetSquare + Move::UP).getIndex()] = Piece(Piece::PAWN, true);
		}
		enPassantSquare = move.targetSquare;
	}

	// Castle
	if (move.castle) {
		if (move.targetSquare.getFile() == 2) {
			piecesPlaced[(move.targetSquare + Move::LEFT * 2).getIndex()] = Piece(Piece::ROOK, (move.targetSquare.getRank() == 0));
			piecesPlaced[(move.targetSquare + Move::RIGHT).getIndex()] = Piece();

			if (move.targetSquare.getRank() == 0)	whiteQueenCastle = true;
			else									blackQueenCastle = true;
		}
		else if (move.targetSquare.getFile() == 6) {
			piecesPlaced[(move.targetSquare + Move::RIGHT).getIndex()] = Piece(Piece::ROOK, (move.targetSquare.getRank() == 0));
			piecesPlaced[(move.targetSquare + Move::LEFT).getIndex()] = Piece();

			if (move.targetSquare.getRank() == 0)	whiteKingCastle = true;
			else									blackKingCastle = true;
		}
	}

	// Undo Special Flag
	whiteKingChecked = move.lastBoardWhiteCheck;
	blackKingChecked = move.lastBoardBlackCheck;

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

	if (piece.isBlack() == board->isWhiteToMove()) return results;
	switch ((piece.getType())) {
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
		if (square.getFile() != 0 && (board->getPiece(targetSquare + LEFT).isEnemy(piece) || (targetSquare + LEFT == board->getEnPassant()))) results.emplace_back(targetSquare + LEFT);
		if (square.getFile() != 7 && (board->getPiece(targetSquare + RIGHT).isEnemy(piece) || (targetSquare + RIGHT == board->getEnPassant()))) results.emplace_back(targetSquare + RIGHT);
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
		if (piece.isWhite()) {
			if (board->getWhiteKingCastle() && !board->isWhiteChecked()) {
				if (!board->getPiece(square + RIGHT) && !board->getPiece(square + RIGHT * 2) && !board->isSquareAttacked(Square(5), false)) results.emplace_back(square + RIGHT * 2);
			}
			if (board->getWhiteQueenCastle() && !board->isWhiteChecked()) {
				if (!board->getPiece(square + LEFT) && !board->getPiece(square + LEFT * 2) && !board->getPiece(square + LEFT * 3) && !board->isSquareAttacked(Square(3), false)) results.emplace_back(square + LEFT * 2);
			}
		}
		else {
			if (board->getBlackKingCastle() && !board->isBlackChecked()) {
				if (!board->getPiece(square + RIGHT) && !board->getPiece(square + RIGHT * 2) && !board->isSquareAttacked(Square(61), true)) results.emplace_back(square + RIGHT * 2);
			}
			if (board->getBlackQueenCastle() && !board->isBlackChecked()) {
				if (!board->getPiece(square + LEFT) && !board->getPiece(square + LEFT * 2) && !board->getPiece(square + LEFT * 3) && !board->isSquareAttacked(Square(59), true)) results.emplace_back(square + LEFT * 2);
			}
		}
	}
		break;
	default:
		break;
	}

	results.erase(
		std::remove_if(results.begin(), results.end(), [&](Move& move) {
			if (!move.targetSquare) return true;

			move.startSquare = square;
			board->makeMove(move);
			bool kingChecked = (piece.isWhite()) ? board->isWhiteChecked() : board->isBlackChecked();
			board->unMakeMove(move);
			return kingChecked;
			}),
		results.end()
	);
	return results;
}
