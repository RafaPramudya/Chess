// Chess.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>

#include <cstring>
#include <cassert>

class Square;
class Piece;
class Move;
class Board;

class Piece {
private:
	uint8_t id;
public:
	constexpr static const char notation[] = {
		'.', 'P', 'N', 'B', 'R', 'Q', 'K', '.',
		'.', 'p', 'n', 'b', 'r', 'q', 'k'
	};

	enum Type : uint8_t
	{
		NONE = 0,
		PAWN = 1,
		KNIGHT = 2,
		BISHOP = 3,
		ROOK = 4,
		QUEEN = 5,
		KING = 6,

		BLACK = 8
	};

	Piece(uint8_t id);
	Piece(Piece::Type type, bool isWhite);
	Piece() : id(NONE) {}

	bool isBlack() const { return id & Type::BLACK; }
	bool isWhite() const { return !isBlack(); }
	bool isEnemy(Piece& other) const { return (isBlack() ^ other.isBlack()) && !(id == 0 || id == 7); }

	char getNotation() const { return notation[id]; }
	uint8_t getId() const { return id; }

	explicit operator bool() const {
		return (id != NONE && id != BLACK);
	}
};

class Square {
private:
	uint8_t index;

	constexpr static const char* notation[] = {
		"A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8",
		"B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8",
		"C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8",
		"D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8",
		"E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8",
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8",
		"G1", "G2", "G3", "G4", "G5", "G6", "G7", "G8",
		"H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8",
		"NULL"
	};

public:
	Square() : index(64) {};
	Square(uint8_t index) : index(index) {};
	Square(char file, uint8_t rank) : index((file - 'a') * 8 + rank) {};
	Square(const char* tile) {
		assert(std::strlen(tile) == 2);

		index = (tile[0] - 'a') * 8 + (tile[1] - '1');
	}

	uint8_t getIndex() const {
		return index;
	}
	uint8_t getRank() const {
		return index / 8;
	}
	uint8_t getFile() const {
		return index % 8;
	}
	const char* getNotation() const {
		return notation[index];
	}

	explicit operator bool() {
		return index < 64;
	}

	bool operator==(const Square& other) const {
		return this->index == other.index;
	}
	Square operator+(const int value) {
		return Square(index + value);
	}
};

class Move {
private:
	static inline Piece encounterObstacle(Square sq, Piece* piecesArray) {
		if (!sq) return Piece();
		auto& piece = piecesArray[sq.getIndex()];

		return (piece.getId() % 8) ? piece : Piece() ;
	};
public:
	Square startSquare;
	Square targetSquare;

	Piece movingPiece;
	bool executed = false;

	enum Direction : int8_t {
		RIGHT = 1,
		LEFT = -1,
		UP = 8,
		DOWN = -8,

		UL = UP + LEFT,
		UR = UP + RIGHT,
		DL = DOWN + LEFT,
		DR = DOWN + RIGHT
	};

	Move() {}
	Move(Square start, Square target, Piece piece) 
		: startSquare(start), targetSquare(target), movingPiece(piece) {}
	Move(Square target) : targetSquare(target) {}

	static std::vector<Move> generateValidMoves(Square square, Piece* piecesArray); 
	static std::vector<Move> generateMoves(Piece* piecesArray, bool whiteToPlay);
};

class Board {
private:
	Piece piecesPlaced[64];

	Square enPassantSquare;
	struct {
		bool whiteKing = false;		// White King-Side Castle
		bool whiteQueen = false;	// White Queen-Side Castle
		bool blackKing = false;		// Black King-Side Castle
		bool blackQueen = false;	// Black Queen-Side Castle
	} castleAvailable;

	uint16_t halfMove = 0;
	uint16_t fullMove = 0;

public:
	constexpr static const char* defaultFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	bool whiteToMove = true;

	Board();
	Board(Piece* pieceArray);
	Board(const char* fenNotation);

	Piece* getPieces() { return piecesPlaced; }
	Piece& getPiece(Square square) { return piecesPlaced[square.getIndex()]; }

	void makeMove(Move& move);
	void unmakeMove(Move& move);

	void printBoard(bool whiteSide = true);
};