// Chess.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include <string>

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
	bool isEnemy(Piece& other) const { return (isBlack() != other.isBlack()) && !(id == 0 || id == 7); }

	char getNotation() const { return notation[id]; }
	uint8_t getId() const { return id; }
	uint8_t getType() const { return id % 8; }

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
	Square(uint8_t file, uint8_t rank) : index(rank * 8 + file) {};
	Square(const char* tile) {
		assert(std::strlen(tile) == 2);

		index = (tile[1] - '1') * 8 + (tile[0] - 'a');
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
public:
	// Packed Data
	struct {
		uint32_t startSquareIdx : 7			= 64; // 0 - 63 data + 1-bit for None
		uint32_t targetSquareIdx : 6		= 0;  // 0 - 63 data
		uint32_t capturedPieceIdx : 4		= 0;  // 0 - 15 Piece::getId()
		uint32_t lastBoardEnPasasantIdx : 7 = 64; // 0 - 63 data + 1-bit for None
		
		// Special Flag
		uint32_t enPassant : 1				= false; // 0 - 1 bool
		uint32_t castle : 1					= false; // 0 - 1 bool
		uint32_t lastBoardWhiteCheck : 1	= false; // 0 - 1 bool
		uint32_t lastBoardBlackCheck : 1	= false; // 0 - 1 bool

		uint32_t lastWhiteKingCastle  : 1	= false; // 0 - 1 bool
		uint32_t lastWhiteQueenCastle : 1	= false; // 0 - 1 bool
		uint32_t lastBlackKingCastle  : 1	= false; // 0 - 1 bool
		uint32_t lastBlackQueenCastle : 1	= false; // 0 - 1 bool
	};

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
	Move(Square start, Square target, Piece piece) {
		startSquareIdx = start.getIndex();
		targetSquareIdx = target.getIndex();
		capturedPieceIdx = piece.getId();
	}
	Move(Square target) {
		targetSquareIdx = target.getIndex();
	}

	// Getters
	Square	getStartSquare()				const { return Square(startSquareIdx); }
	Square	getTargetSquare()				const { return Square(targetSquareIdx); }
	Piece	getCapturedPiece()				const { return Piece(capturedPieceIdx); }
	Square	getLastBoardEnPassantSquare()	const { return Square(lastBoardEnPasasantIdx); }

	// Setters
	void setStartSquare				 (Square square) { startSquareIdx = square.getIndex(); }
	void setTargetSquare			 (Square square) { targetSquareIdx = square.getIndex(); }
	void setCapturedPiece			 (Piece piece)   { capturedPieceIdx = piece.getId(); }
	void setLastBoardEnPassantSquare (Square square) { lastBoardEnPasasantIdx = square.getIndex(); }

	static std::vector<Move> generateValidMoves(Square square, Board* board); 
	static std::vector<Move> generateMoves(Board* board);
};

class Board {
private:
	Piece piecesPlaced[64];

	Square enPassantSquare;

	struct {
		bool whiteKingCastle	: 1 = false;
		bool whiteQueenCastle	: 1 = false;
		bool blackKingCastle	: 1 = false;
		bool blackQueenCastle	: 1 = false;

		bool blackKingChecked	: 1 = false;
		bool whiteKingChecked	: 1 = false;

		bool whiteToMove		: 1 = true;

		bool moveNotationDebug  : 1 = false;
	};
private:
	Square whiteKingSquare;
	Square blackKingSquare;

	uint16_t halfMove = 0;
	uint16_t fullMove = 0;
	
	std::vector<Move> moveRecord;
	std::string moveNotation = "";
public:
	constexpr static const char* defaultFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Board();
	Board(Piece* pieceArray);
	Board(const char* fenNotation);

	Piece* getPieces() { return piecesPlaced; }
	Piece getPiece(Square square) { return (square) ? piecesPlaced[square.getIndex()] : Piece(); }

	void makeMove(Move& move);
	void unMakeMove(Move move);

	void addMoveRecord(Move& move) { moveRecord.emplace_back(move); }
	Move getMoveRecord() { if (moveRecord.empty()) return Move();  auto move = moveRecord.back(); moveRecord.pop_back(); return move; }

	void printBoard(bool whiteSide = true);

	bool isSquareAttacked(Square sq, bool attackerIsWhite);
	void analyzeCheck();
	bool analyzeCheck(bool white);

	bool isWhiteToMove() const { return whiteToMove; }
	void setWhiteToMove(bool flag) { whiteToMove = flag; }

	bool isWhiteChecked() const { return whiteKingChecked; }
	bool isBlackChecked() const { return blackKingChecked; }

	bool getWhiteKingCastle  () const { return whiteKingCastle ; }
	bool getWhiteQueenCastle () const { return whiteQueenCastle; }
	bool getBlackKingCastle	 () const { return blackKingCastle ; }
	bool getBlackQueenCastle () const { return blackQueenCastle; }

	void setWhiteKingCastle  (bool flag) { whiteKingCastle  = flag; }
	void setWhiteQueenCastle (bool flag) { whiteQueenCastle = flag; }
	void setBlackKingCastle  (bool flag) { blackKingCastle  = flag; }
	void setBlackQueenCastle (bool flag) { blackQueenCastle = flag; }

	Square getWhiteKingSquare() const { return whiteKingSquare; }
	Square getBlackKingSquare() const { return blackKingSquare; }

	void setWhiteKingSquare(Square square) { whiteKingSquare = square; }
	void setBlackKingSquare(Square square) { blackKingSquare = square; }

	Square getEnPassant() const { return enPassantSquare; }
	void setEnPassant(const Square& square) { enPassantSquare = square; }
	void resetEnPassant() { enPassantSquare = Square(); }

	void turnNotationDebug(bool flag) { moveNotationDebug = flag; }
	std::string getMoveNotation() const { return moveNotation; }
};