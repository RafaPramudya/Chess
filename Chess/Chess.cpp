#include "Chess.h"

Piece::Piece(uint8_t id)
{
	this->id = id;
}

Piece::Piece(Piece::Type type, bool isWhite)
{
	this->id = isWhite ? type : type + Type::WHITE;
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
	if (!move.startSquare || !move.targetSquare) throw std::exception("Error Move Invalid");

	auto& startPiece = piecesPlaced[move.startSquare.getIndex()];

	piecesPlaced[move.targetSquare.getIndex()] = startPiece;
	piecesPlaced[move.startSquare.getIndex()] = Piece();
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
