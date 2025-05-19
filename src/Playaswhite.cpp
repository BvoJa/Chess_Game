#include "../include/Playaswhite.h"
#include "../include/Constant.h"
#include "../include/Engine.h"

vector<string> Sounds = {
	"Assets/Sound/move-self.mp3",
	"Assets/Sound/capture.mp3",
	"Assets/Sound/move-check.mp3",
	"Assets/Sound/castle.mp3",
	"Assets/Sound/promote.mp3"
};


Engine Amn;

Playaswhite::Playaswhite() {
	font.loadFromFile("Assets/Fonts/Arial.ttf");
	criticalMove = 0;

	for (int i = 0; i <= 4; i++) {
		sBuffer[i].loadFromFile(Sounds[i]);
		sound[i].setBuffer(sBuffer[i]);
		sound[i].setVolume(50);
	}

	playAgain.setSize(Vector2f(200, 50));
	playAgain.setFillColor(sf::Color::Green);
	playAgain.setPosition(300, 220);

	Quit.setSize(sf::Vector2f(200, 50));
	Quit.setFillColor(sf::Color::Red);
	Quit.setPosition(300, 360);

	Save_Game.setSize(sf::Vector2f(200, 50));
	Save_Game.setFillColor(sf::Color::Cyan);
	Save_Game.setPosition(300, 360);

	Load_Game.setSize(sf::Vector2f(200, 50));
	Load_Game.setFillColor(sf::Color(200, 150, 0));
	Load_Game.setPosition(300, 360);

	_settings = 0;

	index = -1;
	pawnPromote = pieceSelected = NumberOfMove = NumberOfUndo = 0;
	isWhiteTurn = 1;
	square_from = { -1000, -1000 };
	square_to = { -1000, -1000 };

	validMoves.clear();
	Black_history.clear();
	White_history.clear();

	settingText.loadFromFile("Assets/images/settingbutton.png");
	settingbutton.setTexture(settingText);
	settingbutton.setPosition(665, 160);
}

Vector2i Playaswhite::getCord(Figure A) {
	return Vector2i((int)A.shape.getPosition().x / SIZE, (int)A.shape.getPosition().y / SIZE);
}

void Playaswhite::PawnLogic(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);

	if (isWhiteTurn) {
		if (cord.y > 0 && Occur[cord.x][cord.y - 1] == 0) {
			move.push_back(Vector2i(cord.x, cord.y - 1));
		}

		if (cord.y >= 2 && A.played == 0 && Occur[cord.x][cord.y - 2] == 0 && Occur[cord.x][cord.y - 1] == 0) {
			move.push_back(Vector2i(cord.x, cord.y - 2));
		}

		if (cord.y > 0 && cord.x < 7 && Occur[cord.x][cord.y] != Occur[cord.x + 1][cord.y - 1] && Occur[cord.x + 1][cord.y - 1] > 0) {
			move.push_back(Vector2i(cord.x + 1, cord.y - 1));
		}

		if (cord.y > 0 && cord.x > 0 && Occur[cord.x][cord.y] != Occur[cord.x - 1][cord.y - 1] && Occur[cord.x - 1][cord.y - 1] > 0) {
			move.push_back(Vector2i(cord.x - 1, cord.y - 1));
		}

		if (cord.x < 7 && cord.y > 0) {
			if (Black_history.size() && Black_history.back()[3] == pawn_main && Black[Black_history.back()[2]].played == 1) {
				int lst_x = Black_history.back()[0];
				int lst_y = Black_history.back()[1];

				if (lst_x == cord.x + 1 && lst_y == cord.y) move.push_back(Vector2i(cord.x + 1, cord.y - 1));
			}
		}

		if (cord.x > 0 && cord.y > 0) {
			if (Black_history.size() && Black_history.back()[3] == pawn_main && Black[Black_history.back()[2]].played == 1) {
				int lst_x = Black_history.back()[0];
				int lst_y = Black_history.back()[1];

				if (lst_x == cord.x - 1 && lst_y == cord.y) move.push_back(Vector2i(cord.x - 1, cord.y - 1));
			}
		}

		return;
	}

	if (cord.y < 7 && BoardState[cord.x][cord.y + 1] == 0) {
		move.push_back(Vector2i(cord.x, cord.y + 1));
	}

	if (cord.y <= 5 && A.played == 0 && BoardState[cord.x][cord.y + 2] == 0 && BoardState[cord.x][cord.y + 1] == 0) {
		move.push_back(Vector2i(cord.x, cord.y + 2));
	}

	if (cord.y < 7 && cord.x < 7 && Occur[cord.x][cord.y] != Occur[cord.x + 1][cord.y + 1] && Occur[cord.x + 1][cord.y + 1] > 0) {
		move.push_back(Vector2i(cord.x + 1, cord.y + 1));
	}

	if (cord.y < 7 && cord.x > 0 && Occur[cord.x][cord.y] != Occur[cord.x - 1][cord.y + 1] && Occur[cord.x - 1][cord.y + 1] > 0) {
		move.push_back(Vector2i(cord.x - 1, cord.y + 1));
	}

	if (cord.x < 7 && cord.y < 7) {
		if (White_history.size() && White_history.back()[3] == pawn_main && White[White_history.back()[2]].played == 1) {
			int lst_x = White_history.back()[0];
			int lst_y = White_history.back()[1];

			if (lst_x == cord.x + 1 && lst_y == cord.y) move.push_back(Vector2i(cord.x + 1, cord.y + 1));
		}
	}

	if (cord.x > 0 && cord.y < 7) {
		if (White_history.size() && White_history.back()[3] == pawn_main && White[White_history.back()[2]].played == 1) {
			int lst_x = White_history.back()[0];
			int lst_y = White_history.back()[1];

			if (lst_x == cord.x - 1 && lst_y == cord.y) move.push_back(Vector2i(cord.x - 1, cord.y + 1));
		}
	}
}

void Playaswhite::KingLogic(Figure A, vector <Vector2i>& move) {
	Vector2i cord = getCord(A);

	for (int i = 0; i < 8; i++) {
		int x = cord.x + Kx[i];
		int y = cord.y + Ky[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
		move.push_back(Vector2i(x, y));
	}
}

void Playaswhite::KnightLogic(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);

	for (int i = 0; i < 8; i++) {
		int x = cord.x + Knx[i];
		int y = cord.y + Kny[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
		move.push_back(Vector2i(x, y));
	}
}

void Playaswhite::Horizontal(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);
	for (int i = cord.x + 1; i < 8; i++) {
		if (Occur[cord.x][cord.y] == Occur[i][cord.y]) break;
		move.push_back(Vector2i(i, cord.y));
		if (Occur[i][cord.y] > 0) break;
	}
	for (int i = cord.x - 1; i >= 0; i--) {
		if (Occur[cord.x][cord.y] == Occur[i][cord.y]) break;
		move.push_back(Vector2i(i, cord.y));
		if (Occur[i][cord.y] > 0) break;
	}
}

void Playaswhite::Vertical(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);
	for (int i = cord.y + 1; i < 8; i++) {
		if (Occur[cord.x][cord.y] == Occur[cord.x][i]) break;
		move.push_back(Vector2i(cord.x, i));
		if (Occur[cord.x][i] > 0) break;
	}
	for (int i = cord.y - 1; i >= 0; i--) {
		if (Occur[cord.x][cord.y] == Occur[cord.x][i]) break;
		move.push_back(Vector2i(cord.x, i));
		if (Occur[cord.x][i] > 0) break;
	}
}

void Playaswhite::Diagonal(Figure A, vector<Vector2i>& move) {
	Vector2i cord = getCord(A);
	int i, j; i = cord.x, j = cord.y;

	while (true) {
		i--; j--;
		if (i < 0 || j < 0) break;
		if (Occur[i][j] != Occur[cord.x][cord.y]) {
			move.push_back(Vector2i(i, j));
		}

		if (Occur[i][j] > 0) break;
	}

	i = cord.x, j = cord.y;

	while (true) {
		i--; j++;
		if (i < 0 || j > 7) break;
		if (Occur[i][j] != Occur[cord.x][cord.y]) {
			move.push_back(Vector2i(i, j));
		}
		if (Occur[i][j] > 0) break;
	}


	i = cord.x, j = cord.y;

	while (true) {
		i++; j--;
		if (i > 7 || j < 0) break;
		if (Occur[i][j] != Occur[cord.x][cord.y]) {
			move.push_back(Vector2i(i, j));
		}
		if (Occur[i][j] > 0) break;
	}

	i = cord.x, j = cord.y;

	while (true) {
		i++; j++;
		if (i > 7 || j > 7) break;
		if (Occur[i][j] != Occur[cord.x][cord.y]) {
			move.push_back(Vector2i(i, j));
		}
		if (Occur[i][j] > 0) break;
	}
}

void Playaswhite::drawUndoRedo(RenderWindow& window) {
	Texture redo, undo, reset;

	redo.loadFromFile("Assets/images/redo.png");
	undo.loadFromFile("Assets/images/undo.png");
	reset.loadFromFile("Assets/images/reset.png");

	Redo.setTexture(redo);
	Undo.setTexture(undo);
	Reset.setTexture(reset);

	float targetSize = 60.0f;
	float scaleX = targetSize / undo.getSize().x;
	float scaleY = targetSize / undo.getSize().y;
	Undo.setScale(scaleX, scaleY);


	scaleX = targetSize / redo.getSize().x;
	scaleY = targetSize / redo.getSize().y;
	Redo.setScale(scaleX, scaleY);

	scaleX = targetSize / reset.getSize().x;
	scaleY = targetSize / reset.getSize().y;
	Reset.setScale(scaleX, scaleY);


	Undo.setPosition(629, 510);
	Redo.setPosition(712, 510);
	Reset.setPosition(670, 450);

	Vector2i pos = Mouse::getPosition(window);

	Convert(Reset, pos, 150);
	Convert(Undo, pos, 150);
	Convert(Redo, pos, 150);

	window.draw(Reset);
	window.draw(Undo);
	window.draw(Redo);
}

bool Playaswhite::isValidMove(Figure A, Vector2i pos) {
	vector<Vector2i> move;
	getValidMove(A, move);

	for (Vector2i x : move) if (x == pos && KingSafe(x)) return 1;

	vector<Vector2i> a;

	if (A.type == king_main && A.played == 0 && KingSafe(getCord(A))) {
		Castling(a);
	}

	for (auto x : a) {
		if (pos == x) return 1;
	}
	return 0;
}

void Playaswhite::getValidMove(Figure A, vector<Vector2i>& move) {
	if (A.type == pawn_main) {
		PawnLogic(A, move);
		return;
	}

	if (A.type == knight_main) {
		KnightLogic(A, move);
		return;
	}

	if (A.type == rook_main) {
		Vertical(A, move);
		Horizontal(A, move);
		return;
	}

	if (A.type == bishop_main) {
		Diagonal(A, move);
		return;
	}

	if (A.type == king_main) {
		KingLogic(A, move);
		return;
	}

	Vertical(A, move);
	Horizontal(A, move);
	Diagonal(A, move);
}

bool Playaswhite::TakePiece(Vector2i pos) {
	if (isWhiteTurn && Black_history.size() && Black_history.back()[3] == pawn_main && White[index].type == pawn_main) {
		int lst_x = Black_history.back()[0];
		int lst_y = Black_history.back()[1];
		int id = Black_history.back()[2];

		if (Black[id].played == 1 && lst_x == pos.x && lst_y == pos.y + 1) {
			Black[id].shape.setPosition(-400, -400);
			return 1;
		}
	}
	else if (isWhiteTurn == 0 && White_history.size() && White_history.back()[3] == pawn_main && Black[index].type == pawn_main) {
		int lst_x = White_history.back()[0];
		int lst_y = White_history.back()[1];
		int id = White_history.back()[2];

		if (White[id].played == 1 && lst_x == pos.x && lst_y == pos.y - 1) {
			White[id].shape.setPosition(-400, -400);
			return 1;
		}
	}

	bool ok = 0;
	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(White[i]);
		if (cord != pos) continue;
		White[i].SetPosition(-500, -500);
		ok = 1;
	}

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(Black[i]);
		if (cord != pos) continue;
		Black[i].SetPosition(-500, -500);
		ok = 1;
	}

	return ok;
}

void Playaswhite::UpdBoardState() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			BoardState[i][j] = 0;
			Occur[i][j] = 0;
		}
	}

	if (isWhiteTurn) {
		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(Black[i]);
			SaveBoard[NumberOfMove][i + 16] = { cord.x, cord.y, Black[i].type, Black[i].played };

			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
			BoardState[cord.x][cord.y] = Black[i].type + 200;
			Occur[cord.x][cord.y] = 1;
		}

		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(White[i]);
			SaveBoard[NumberOfMove][i] = { cord.x, cord.y, White[i].type, White[i].played };

			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
			BoardState[cord.x][cord.y] = White[i].type + 100;
			Occur[cord.x][cord.y] = 2;
		}

		SaveBoard[NumberOfMove][32][0] = criticalMove;
	}
	else {
		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(White[i]);
			SaveBoard[NumberOfMove][i] = { cord.x, cord.y, White[i].type, White[i].played };

			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
			BoardState[cord.x][cord.y] = White[i].type + 100;
			Occur[cord.x][cord.y] = 2;
		}

		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(Black[i]);
			SaveBoard[NumberOfMove][i + 16] = { cord.x, cord.y, Black[i].type, Black[i].played };

			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
			BoardState[cord.x][cord.y] = Black[i].type + 200;
			Occur[cord.x][cord.y] = 1;
		}

		SaveBoard[NumberOfMove][32][0] = criticalMove;
	}
}

void Playaswhite::ResetRedo() {
	if (NumberOfUndo) {
		for (int i = 0; i < 32; i++) {
			if (SaveBoard[NumberOfMove][i] != store[NumberOfUndo][i]) {
				NumberOfUndo = 0;
				return;
			}
		}
		NumberOfUndo--;
	}
}

bool Playaswhite::check() {
	if (isWhiteTurn) {
		Vector2i cord = getCord(White[4]);
		if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) return 1;

		for (int i = cord.x - 1; i >= 0; i--) {
			if (Occur[i][cord.y] == 0) continue;
			if (Occur[i][cord.y] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][cord.y] == queen_main + 200 || BoardState[i][cord.y] == rook_main + 200) return 1;
			if (Occur[i][cord.y] > 0) break;
		}

		for (int i = cord.x + 1; i <= 7; i++) {
			if (Occur[i][cord.y] == 0) continue;
			if (Occur[i][cord.y] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][cord.y] == queen_main + 200 || BoardState[i][cord.y] == rook_main + 200) return 1;
			if (Occur[i][cord.y] > 0) break;
		}

		for (int i = cord.y + 1; i <= 7; i++) {
			if (Occur[cord.x][i] == 0) continue;
			if (Occur[cord.x][i] == Occur[cord.x][cord.y]) break;
			if (BoardState[cord.x][i] == queen_main + 200 || BoardState[cord.x][i] == rook_main + 200) return 1;
			if (Occur[cord.x][i] > 0) break;
		}

		for (int i = cord.y - 1; i >= 0; i--) {
			if (Occur[cord.x][i] == 0) continue;
			if (Occur[cord.x][i] == Occur[cord.x][cord.y]) break;
			if (BoardState[cord.x][i] == queen_main + 200 || BoardState[cord.x][i] == rook_main + 200) return 1;
			if (Occur[cord.x][i] > 0) break;
		}

		int i, j; i = cord.x, j = cord.y;

		while (true) {
			i--; j--;
			if (i < 0 || j < 0) break;
			if (Occur[i][j] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}

		i = cord.x, j = cord.y;

		while (true) {
			i--; j++;
			if (i < 0 || j > 7) break;
			if (Occur[i][j] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}


		i = cord.x, j = cord.y;

		while (true) {
			i++; j--;
			if (i > 7 || j < 0) break;
			if (Occur[i][j] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}

		i = cord.x, j = cord.y;

		while (true) {
			i++; j++;
			if (i > 7 || j > 7) break;
			if (Occur[i][j] == Occur[cord.x][cord.y]) break;
			if (BoardState[i][j] == bishop_main + 200 || BoardState[i][j] == queen_main + 200) return 1;
			if (Occur[i][j] > 0) break;
		}


		for (int i = 0; i < 8; i++) {
			int x = cord.x - Knx[i];
			int y = cord.y - Kny[i];
			if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
			if (BoardState[x][y] == knight_main + 200) return 1;
		}

		for (int i = 0; i < 8; i++) {
			int x = cord.x - Kx[i];
			int y = cord.y - Ky[i];
			if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
			if (BoardState[x][y] == king_main + 200) return 1;
		}

		if (cord.x >= 1 && cord.y >= 1 && BoardState[cord.x - 1][cord.y - 1] == pawn_main + 200) return 1;
		if (cord.x < 7 && cord.y >= 1 && BoardState[cord.x + 1][cord.y - 1] == pawn_main + 200) return 1;

		return 0;
	}

	Vector2i cord = getCord(Black[4]);
	if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) return 1;

	for (int i = cord.x - 1; i >= 0; i--) {
		if (Occur[i][cord.y] == 0) continue;
		if (Occur[i][cord.y] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][cord.y] == queen_main + 100 || BoardState[i][cord.y] == rook_main + 100) return 1;
		if (Occur[i][cord.y] > 0) break;
	}

	for (int i = cord.x + 1; i <= 7; i++) {
		if (Occur[i][cord.y] == 0) continue;
		if (Occur[i][cord.y] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][cord.y] == queen_main + 100 || BoardState[i][cord.y] == rook_main + 100) return 1;
		if (Occur[i][cord.y] > 0) break;
	}

	for (int i = cord.y + 1; i <= 7; i++) {
		if (Occur[cord.x][i] == 0) continue;
		if (Occur[cord.x][i] == Occur[cord.x][cord.y]) break;
		if (BoardState[cord.x][i] == queen_main + 100 || BoardState[cord.x][i] == rook_main + 100) return 1;
		if (Occur[cord.x][i] > 0) break;
	}

	for (int i = cord.y - 1; i >= 0; i--) {
		if (Occur[cord.x][i] == 0) continue;
		if (Occur[cord.x][i] == Occur[cord.x][cord.y]) break;
		if (BoardState[cord.x][i] == queen_main + 100 || BoardState[cord.x][i] == rook_main + 100) return 1;
		if (Occur[cord.x][i] > 0) break;
	}

	int i, j; i = cord.x, j = cord.y;

	while (true) {
		i--; j--;
		if (i < 0 || j < 0) break;
		if (Occur[i][j] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	i = cord.x, j = cord.y;

	while (true) {
		i--; j++;
		if (i < 0 || j > 7) break;
		if (Occur[i][j] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}


	i = cord.x, j = cord.y;

	while (true) {
		i++; j--;
		if (i > 7 || j < 0) break;
		if (Occur[i][j] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	i = cord.x, j = cord.y;

	while (true) {
		i++; j++;
		if (i > 7 || j > 7) break;
		if (Occur[i][j] == Occur[cord.x][cord.y]) break;
		if (BoardState[i][j] == bishop_main + 100 || BoardState[i][j] == queen_main + 100) return 1;
		if (Occur[i][j] > 0) break;
	}

	for (int i = 0; i < 8; i++) {
		int x = cord.x - Knx[i];
		int y = cord.y - Kny[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
		if (BoardState[x][y] == knight_main + 100) return 1;
	}

	for (int i = 0; i < 8; i++) {
		int x = cord.x - Kx[i];
		int y = cord.y - Ky[i];
		if (x < 0 || y < 0 || x > 7 || y > 7 || Occur[x][y] == Occur[cord.x][cord.y]) continue;
		if (BoardState[x][y] == king_main + 100) return 1;
	}

	if (cord.x < 7 && cord.y < 7 && BoardState[cord.x + 1][cord.y + 1] == pawn_main + 100) return 1;
	if (cord.x >= 1 && cord.y < 7 && BoardState[cord.x - 1][cord.y + 1] == pawn_main + 100) return 1;

	return 0;
}

bool Playaswhite::KingSafe(Vector2i temp) {
	bool ok = 1;

	if (isWhiteTurn) {
		Vector2f real_position = White[index].shape.getPosition();
		White[index].shape.setPosition(temp.x * SIZE, temp.y * SIZE);
		UpdBoardState();

		Occur[temp.x][temp.y] = 2;
		if (check()) ok = 0;

		White[index].shape.setPosition(real_position);
		UpdBoardState();
		return ok;
	}

	Vector2f real_position = Black[index].shape.getPosition();
	Black[index].shape.setPosition(temp.x * SIZE, temp.y * SIZE);
	UpdBoardState();

	Occur[temp.x][temp.y] = 1;
	if (check()) ok = 0;

	Black[index].shape.setPosition(real_position);
	UpdBoardState();

	return ok;
}

void Playaswhite::Castling(vector <Vector2i>& move) {
	if (isWhiteTurn) {
		Vector2i king = getCord(White[4]);

		if (White[0].played == 0) {
			Vector2i rook = getCord(White[0]);
			int l = rook.x;
			int r = king.x;
			bool ok = 1;
			if (rook.x < 0 || rook.y < 0) ok = 0;

			for (int i = l + 1; i < r; i++)  if (Occur[i][king.y] != 0) {
				ok = 0; break;
			}

			if (ok && KingSafe(Vector2i(king.x - 1, king.y)) && KingSafe(Vector2i(king.x - 2, king.y))) {
				move.push_back(Vector2i(king.x - 2, king.y));
			}
		}

		if (White[7].played == 0) {
			Vector2i rook = getCord(White[7]);
			int l = king.x;
			int r = rook.x;

			if (rook.x < 0 || rook.y < 0) return;

			for (int i = l + 1; i < r; i++)  if (Occur[i][king.y] != 0) {
				return;
			}

			if (KingSafe(Vector2i(king.x + 1, king.y)) && KingSafe(Vector2i(king.x + 2, king.y))) {
				move.push_back(Vector2i(king.x + 2, king.y));
			}
		}

		return;
	}

	Vector2i king = getCord(Black[4]);

	if (Black[0].played == 0) {
		Vector2i rook = getCord(Black[0]);
		int l = rook.x;
		int r = king.x;

		bool ok = 1;
		if (rook.x < 0 || rook.y < 0) ok = 0;

		for (int i = l + 1; i < r; i++)  if (Occur[i][king.y] != 0) {
			ok = 0; break;
		}

		if (ok && KingSafe(Vector2i(king.x - 1, king.y)) && KingSafe(Vector2i(king.x - 2, king.y))) {
			move.push_back(Vector2i(king.x - 2, king.y));
		}
	}

	if (Black[7].played == 0) {
		Vector2i rook = getCord(Black[7]);
		int l = king.x;
		int r = rook.x;
		if (rook.x < 0 || rook.y < 0) return;

		for (int i = l + 1; i < r; i++)  if (Occur[i][king.y] != 0) return;

		if (KingSafe(Vector2i(king.x + 1, king.y)) && KingSafe(Vector2i(king.x + 2, king.y))) {
			move.push_back(Vector2i(king.x + 2, king.y));
		}
	}
}

void Playaswhite::drawValidMoves(RenderWindow& window, const std::vector<Vector2i>& validmoves) {
	int TILE_SIZE = SIZE;
	int TILE_OFFSET = 24;

	CircleShape highlight(13);
	highlight.setFillColor(Color(0, 255, 0, 100));


	for (const auto& move : validmoves) if (KingSafe(move)) {
		highlight.setFillColor(Color(0, 255, 0, 100));
		highlight.setPosition(move.x * TILE_SIZE + TILE_OFFSET, move.y * TILE_SIZE + TILE_OFFSET);
		if (Occur[move.x][move.y]) 	highlight.setFillColor(Color::Red);
		window.draw(highlight);
	}

	vector<Vector2i> a;
	if (isWhiteTurn && White[index].type == king_main && White[index].played == 0 && KingSafe(getCord(White[index]))) {
		Castling(a);
	}
	else if (isWhiteTurn == 0 && Black[index].type == king_main && Black[index].played == 0 && KingSafe(getCord(Black[index]))) {
		Castling(a);
	}

	for (auto move : a) {
		highlight.setPosition(move.x * TILE_SIZE + TILE_OFFSET, move.y * TILE_SIZE + TILE_OFFSET);
		window.draw(highlight);
	}
}

void Playaswhite::UpdHistoryWhite(Figure A, Vector2i pos) {
	array<int, 4> lastMove;

	lastMove[0] = pos.x;
	lastMove[1] = pos.y;
	lastMove[2] = index;
	lastMove[3] = A.type;

	White_history.push_back(lastMove);
}

void Playaswhite::UpdHistoryBlack(Figure A, Vector2i pos) {
	array<int, 4> lastMove;

	lastMove[0] = pos.x;
	lastMove[1] = pos.y;
	lastMove[2] = index;
	lastMove[3] = A.type;

	Black_history.push_back(lastMove);
}

void Playaswhite::drawPawnPromote() {
	float promotionBoardX = 700.0f;
	float promotionBoardY = 45.0f;
	float iconSize = 60.0f;

	promotionBoardX = 45;
	promotionBoardY = 0;

	sf::RectangleShape background(sf::Vector2f(iconSize, iconSize * 5 + 20));
	background.setPosition(promotionBoardX, promotionBoardY);
	background.setFillColor(sf::Color(240, 240, 240));

	promotion.draw(background);

	Vector2i cord = getCord(White[index]);

	for (int i = 1; i <= 4; i++) {
		sample[i].setTexture(white[i]);
		sample[i].setScale(iconSize / sample[i].getLocalBounds().width, iconSize / sample[i].getLocalBounds().height);
		sample[i].setPosition(promotionBoardX, promotionBoardY + (i - 1) * iconSize);
		promotion.draw(sample[i]);
	}

	cancelText.setFont(font);
	cancelText.setString("X");
	cancelText.setCharacterSize(40);
	cancelText.setFillColor(sf::Color(150, 150, 150));
	cancelText.setPosition(promotionBoardX + 15, promotionBoardY + 4 * iconSize + 10);
	promotion.draw(cancelText);

	promotion.display();
}

int Playaswhite::PawnPromotion(Event event) {
	Vector2i pos = Mouse::getPosition(promotion);

	for (int i = 1; i <= 4; i++) {
		if (sample[i].getGlobalBounds().contains(pos.x, pos.y))
			sample[i].setColor(sf::Color(255, 255, 255, 115));
		else
			sample[i].setColor(sf::Color(255, 255, 255, 255));
	}

	if (cancelText.getGlobalBounds().contains(pos.x, pos.y)) cancelText.setStyle(Text::Bold);
	else cancelText.setStyle(Text::Regular);

	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
		for (int i = 1; i <= 4; i++) {
			if (sample[i].getGlobalBounds().contains((float)pos.x, (float)pos.y)) {
				pawnPromote = 0;
				return i;
			}
		}

		if (cancelText.getGlobalBounds().contains(pos.x, pos.y)) {
			pawnPromote = 0;
			square_from = { -1000, -1000 };
			square_to = { -1000, -1000 };
			return 0; 
		}
	}

	return -1;
}

void Playaswhite::UseUndo() {
	NumberOfUndo++;
	countState[getState()]--;

	for (int i = 0; i <= 32; i++)
		store[NumberOfUndo][i] = SaveBoard[NumberOfMove][i];

	NumberOfMove--;

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	criticalMove = SaveBoard[NumberOfMove][32][0];
	UpdBoardState();

	countState[getState()]--;
	NumberOfUndo++;

	for (int i = 0; i <= 32; i++)
		store[NumberOfUndo][i] = SaveBoard[NumberOfMove][i];

	NumberOfMove--;

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	criticalMove = SaveBoard[NumberOfMove][32][0];
	UpdBoardState();
	for (int i = 0; i < 16; i++) {
		Blackdrag[i] = Black[i];
		Whitedrag[i] = White[i];
	}
}

void Playaswhite::UseRedo() {
	NumberOfMove++;
	for (int i = 0; i <= 32; i++)
		SaveBoard[NumberOfMove][i] = store[NumberOfUndo][i];
	NumberOfUndo--;

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	criticalMove = SaveBoard[NumberOfMove][32][0];
	UpdBoardState();
	countState[getState()]++;

	NumberOfMove++;
	for (int i = 0; i <= 32; i++)
		SaveBoard[NumberOfMove][i] = store[NumberOfUndo][i];
	NumberOfUndo--;


	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	criticalMove = SaveBoard[NumberOfMove][32][0];
	UpdBoardState();
	countState[getState()]++;
	for (int i = 0; i < 16; i++) {
		Blackdrag[i] = Black[i];
		Whitedrag[i] = White[i];
	}
}

void Playaswhite::UndoRedo(Event event, RenderWindow& window) {
	Vector2i pos = Mouse::getPosition(window);

	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Undo.getGlobalBounds().contains(pos.x, pos.y) && NumberOfMove > 1) {
		UseUndo();
	}

	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Redo.getGlobalBounds().contains(pos.x, pos.y) && NumberOfUndo > 1) {
		UseRedo();
	}

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Left && NumberOfMove > 1) {
			UseUndo();
		}

		if (event.key.code == sf::Keyboard::Right && NumberOfUndo > 1) {
			UseRedo();
		}
	}
}

void Playaswhite::ResetAll() {
	index = -1;
	pawnPromote = pieceSelected = NumberOfMove = NumberOfUndo = 0;
	isWhiteTurn = 1; isGameEnd = 0;
	validMoves.clear();
	Black_history.clear();
	White_history.clear();
	countState.clear();
	criticalMove = 0;
	square_from = square_to = { -10000, -10000 };
	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	UpdBoardState();
	countState[getState()]++;
	for (int i = 0; i < 16; i++) {
		Blackdrag[i] = Black[i];
		Whitedrag[i] = White[i];
	}
}

void Playaswhite::GetDiffculty(bool _easy, bool _medium, bool _hard) {
	Easy = _easy; Medium = _medium; Hard = _hard;
}

long long Playaswhite::getState() {
	long long state = 0;

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(White[i]);
		if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
		int square = cord.x * 8 + cord.y;
		if (square >= 64) continue;

		state = (((state * 97LL + cord.x) * 97LL + cord.y) + 1) * 97LL + White[i].type;
	}

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(Black[i]);
		if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
		int square = cord.x * 8 + cord.y;
		if (square >= 64) continue;

		state = (((state * 97LL + cord.x) * 97LL + cord.y) + 2) * 97LL + Black[i].type;
	}
	state = (state * 97 + (White[4].played > 0)) * 97LL + (Black[4].played > 0);

	return state;
}

int Playaswhite::EndGame() {

	if (countState[getState()] == 3) {
		return repetition_main;
	}

	if (criticalMove == 100) return fifty_main;

	int cnt = 0;

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(White[i]);
		if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;

		cnt++;
	}

	bool ok = (cnt == 2);

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(Black[i]);
		if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;

		cnt++;
	}

	if (cnt == 3) {
		for (int i = 0; i < 16; i++) if (White[i].type == bishop_main || White[i].type == knight_main) {
			Vector2i cord = getCord(White[i]);
			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
			return insufficient_main;
		}

		for (int i = 0; i < 16; i++) if (Black[i].type == bishop_main || Black[i].type == knight_main) {
			Vector2i cord = getCord(White[i]);
			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
			return insufficient_main;
		}
	}

	if (cnt == 2) return insufficient_main;


	if (cnt == 4 && ok) {
		short odd = -1;
		for (int i = 0; i < 16; i++) if (White[i].type == bishop_main) {
			Vector2i cord = getCord(White[i]);
			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;

			odd = (cord.x + cord.y) & 1;
		}

		for (int i = 0; i < 16; i++) if (Black[i].type == bishop_main) {
			Vector2i cord = getCord(Black[i]);
			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;

			if (((cord.x + cord.y) & 1) == odd) return insufficient_main;
		}
	}


	if (isWhiteTurn) {
		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(White[i]);
			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;

			vector<Vector2i> valid;
			index = i;

			getValidMove(White[i], valid);

			for (auto move : valid) {
				if (KingSafe(move)) return 0;
			}
		}
		if (check()) return checkmate_main;
		return stalemate_main;
	}

	for (int i = 0; i < 16; i++) {
		Vector2i cord = getCord(Black[i]);
		if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;

		index = i;

		vector<Vector2i> valid;
		getValidMove(Black[i], valid);

		for (auto move : valid) {
			if (KingSafe(move)) return 0;
		}
	}
	if (check()) return checkmate_main;

	return stalemate_main;
}

bool Playaswhite::MakeMove(int x, int y, int promoted) {
	Vector2i temp; temp.x = x; temp.y = y;

	bool ok = 0;

	if (isWhiteTurn) {
		if (isValidMove(White[index], temp)) {
			ok = 1;
			Vector2i cord = getCord(White[index]);

			if (White[index].type == pawn_main && temp.y == 0) {
				pawnPromote = 1;
				target = temp;
				square_from = cord;
				square_to = temp;


				promotion.create(VideoMode(100, 320), "Promotion");
				promotion.setPosition(Vector2i(400, 300));
				Image icon;
				icon.loadFromFile("Assets/images/pawnpromotion.png");
				promotion.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
				return 1;
			}
			square_from = cord;
			square_to = temp;
			if (White[index].type == pawn_main) criticalMove = 0;
			else criticalMove++;

			if (TakePiece(temp)) sound[1].play(), criticalMove = 0;
			else if (White[index].played == 0 && index == 4) {
				if (temp.x == cord.x + 2 && temp.y == cord.y) White[7].shape.setPosition((temp.x - 1) * SIZE, temp.y * SIZE), sound[3].play();
				else if (temp.x == cord.x - 2 && temp.y == cord.y) White[0].shape.setPosition((temp.x + 1) * SIZE, temp.y * SIZE), sound[3].play();
				else sound[0].play();
			}
			else sound[0].play();

			if (White[index].played == 0 && index == 4) {
				if (temp.x == cord.x + 2 && temp.y == cord.y) White[7].shape.setPosition((temp.x - 1) * SIZE, temp.y * SIZE), White[7].played++;
				else if (temp.x == cord.x - 2 && temp.y == cord.y) White[0].shape.setPosition((temp.x + 1) * SIZE, temp.y * SIZE), White[0].played++;
			}

			TakePiece(temp);

			UpdHistoryWhite(White[index], temp);

			White[index].shape.setPosition(temp.x * SIZE, temp.y * SIZE);
			White[index].played++;
			NumberOfMove++;

			UpdBoardState();
			ResetRedo();
			isWhiteTurn = 0;
			if (check()) sound[2].play();

			countState[getState()]++;
			isGameEnd = EndGame();
		}
		pieceSelected = 0;
		if (pawnPromote) {
			square_from = { -1000, -1000 };
			square_to = { -1000, -1000 };
		}
		pawnPromote = 0;
		promotion.close();
		return ok;
	}

	Vector2i AI;  AI.x = x; AI.y = y;
	Vector2i cord = getCord(Black[index]);


	if (Black[index].type == pawn_main && AI.y == 7) {
		int x = promoted;
		sound[4].play();
		criticalMove = 0;

		square_from = cord;
		square_to = temp;

		if (x == rook_main) Black[index].shape.setTexture(black[rook_main]), Black[index].type = rook_main;
		if (x == queen_main) Black[index].shape.setTexture(black[queen_main]), Black[index].type = queen_main;
		if (x == bishop_main) Black[index].shape.setTexture(black[bishop_main]), Black[index].type = bishop_main;
		if (x == knight_main) Black[index].shape.setTexture(black[knight_main]), Black[index].type = knight_main;

		TakePiece(AI);

		UpdHistoryBlack(Black[index], AI);
		Black[index].shape.setPosition(AI.x * SIZE, AI.y * SIZE);
		Black[index].played++;
		NumberOfMove++;
		UpdBoardState();
		ResetRedo();

		isWhiteTurn = 1;
		if (check()) sound[2].play();

		countState[getState()]++;
		isGameEnd = EndGame();
		return 1;
	}

	if (isValidMove(Black[index], AI) == 0) {
		cout << "DCM AI lo3\n";
		cout << index << '\n';
		cout << AI.x << ' ' << AI.y << ' ' << promoted << '\n';

		return 0;
	}

	square_from = cord;
	square_to = temp;

	if (Black[index].type == pawn_main) criticalMove = 0;
	else criticalMove++;

	if (TakePiece(AI)) sound[1].play(), criticalMove = 0;
	else if (Black[index].played == 0 && index == 4) {
		if (AI.x == cord.x + 2 && AI.y == cord.y) Black[7].shape.setPosition((AI.x - 1) * SIZE, AI.y * SIZE), sound[3].play();
		else if (AI.x == cord.x - 2 && AI.y == cord.y) Black[0].shape.setPosition((AI.x + 1) * SIZE, AI.y * SIZE), sound[3].play();
		else sound[0].play();
	}
	else sound[0].play();

	if (Black[index].played == 0 && index == 4) {
		if (AI.x == cord.x + 2 && AI.y == cord.y) Black[7].shape.setPosition((AI.x - 1) * SIZE, AI.y * SIZE), Black[7].played++;
		else if (AI.x == cord.x - 2 && AI.y == cord.y) Black[0].shape.setPosition((AI.x + 1) * SIZE, AI.y * SIZE), Black[0].played++;
	}

	TakePiece(AI);

	UpdHistoryBlack(Black[index], AI);

	NumberOfMove++;
	Black[index].shape.setPosition(AI.x * SIZE, AI.y * SIZE);
	Black[index].played++;

	UpdBoardState();
	ResetRedo();
	isWhiteTurn = 1;

	if (check()) sound[2].play();

	countState[getState()]++;
	isGameEnd = EndGame();
	return 1;
}

void Playaswhite::loadGame(const string& filename) {
	if (filename == "") return;

	ifstream fin(filename);

	if (!fin.is_open()) {
		return;
	}
	ResetAll();
	bool Type;
	fin >> isWhiteTurn >> NumberOfMove >> criticalMove >> Type;
	if (fin.eof()) return;

	for (int i = 0; i <= NumberOfMove; i++) {
		long long state = 0;

		for (int j = 0; j < 16; j++) {
			array <int, 4> lst_move;
			fin >> lst_move[0] >> lst_move[1] >> lst_move[2] >> lst_move[3];

			if (Type) {
				lst_move[0] = 7 - lst_move[0];
				lst_move[1] = 7 - lst_move[1];
			}

			SaveBoard[i][j] = lst_move;

			int square = lst_move[0] * 8 + lst_move[1];
			if (square >= 64) continue;

			state = (((state * 97LL + lst_move[0]) * 97LL + lst_move[1]) + 1) * 97LL + lst_move[2];
		}

		for (int j = 0; j < 16; j++) {
			array <int, 4> lst_move;
			fin >> lst_move[0] >> lst_move[1] >> lst_move[2] >> lst_move[3];

			if (Type) {
				lst_move[0] = 7 - lst_move[0];
				lst_move[1] = 7 - lst_move[1];
			}

			SaveBoard[i][j + 16] = lst_move;
			int square = lst_move[0] * 8 + lst_move[1];
			if (square >= 64) continue;

			state = (((state * 97LL + lst_move[0]) * 97LL + lst_move[1]) + 2) * 97LL + lst_move[2];
		}

		state = (state * 97 + (SaveBoard[i][4][3] > 0)) * 97LL + (SaveBoard[i][20][3] > 0);

		countState[state]++;
	}

	int sz; fin >> sz;
	for (int i = 0; i < sz; i++) {
		array <int, 4> lst_move;
		fin >> lst_move[0] >> lst_move[1] >> lst_move[2] >> lst_move[3];
		if (Type) {
			lst_move[0] = 7 - lst_move[0];
			lst_move[1] = 7 - lst_move[1];
		}
		White_history.push_back(lst_move);
	}

	fin >> sz;

	for (int i = 0; i < sz; i++) {
		array <int, 4> lst_move;
		fin >> lst_move[0] >> lst_move[1] >> lst_move[2] >> lst_move[3];
		if (Type) {
			lst_move[0] = 7 - lst_move[0];
			lst_move[1] = 7 - lst_move[1];
		}
		Black_history.push_back(lst_move);
	}


	fin.close();

	if (NumberOfMove > 0) {
		for (int i = 0; i < 16; i++) {
			array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
			White[i].type = lst_move[2];
			White[i].shape.setTexture(white[lst_move[2]]);
			White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
			White[i].played = lst_move[3];
		}

		for (int i = 0; i < 16; i++) {
			array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
			Black[i].type = lst_move[2];
			Black[i].shape.setTexture(black[lst_move[2]]);
			Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
			Black[i].played = lst_move[3];
		}
	}


	UpdBoardState();
	for (int i = 0; i < 16; i++) {
		Blackdrag[i] = Black[i];
		Whitedrag[i] = White[i];
	}
}

void Playaswhite::SaveGame(const string& filename) {
	if (filename == "") return;
	ofstream fout(filename);

	fout << isWhiteTurn << ' ' << NumberOfMove << ' ' << criticalMove << " 0\n";

	for (int i = 0; i <= NumberOfMove; i++) {
		for (int j = 0; j < 16; j++) {
			array<int, 4> lst_move = SaveBoard[i][j];
			fout << lst_move[0] << ' ' << lst_move[1] << ' ' << lst_move[2] << ' ' << lst_move[3] << '\n';
		}

		for (int j = 0; j < 16; j++) {
			array<int, 4> lst_move = SaveBoard[i][j + 16];
			fout << lst_move[0] << ' ' << lst_move[1] << ' ' << lst_move[2] << ' ' << lst_move[3] << '\n';
		}
	}

	fout << White_history.size() << '\n';

	for (int i = 0; i < White_history.size(); i++) {
		array <int, 4> lst_move = White_history[i];
		fout << lst_move[0] << ' ' << lst_move[1] << ' ' << lst_move[2] << ' ' << lst_move[3] << '\n';
	}

	fout << Black_history.size() << '\n';

	for (int i = 0; i < Black_history.size(); i++) {
		array <int, 4> lst_move = Black_history[i];
		fout << lst_move[0] << ' ' << lst_move[1] << ' ' << lst_move[2] << ' ' << lst_move[3] << '\n';
	}

	fout.close();
}

void Playaswhite::PrintSaveFile() {
	GameSave.create(VideoMode(800, 400), "Save Game");
	GameSave.setPosition(Vector2i(400, 300));
	sf::Image icon;
	icon.loadFromFile("Assets/images/save.png");
	GameSave.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	RectangleShape inputBox(sf::Vector2f(400, 50));
	inputBox.setFillColor(sf::Color(200, 200, 200));
	inputBox.setOutlineThickness(2);
	inputBox.setOutlineColor(sf::Color(255, 150, 0));
	inputBox.setPosition(200, 200);

	sf::Text instruction("Enter filename and press Enter to save:", font, 24);
	instruction.setFillColor(sf::Color::White);
	instruction.setPosition(180, 150);
	Text filenameText;

	filenameText = Text("", font, 24);
	filenameText.setFillColor(sf::Color::Black);
	filenameText.setPosition(210, 210);

	std::string filename;
	bool isFileSaved = false;

	// Con trỏ nhấp nháy
	bool showCursor = true; // Biến để bật/tắt con trỏ
	sf::Clock cursorClock;  // Đếm thời gian để nhấp nháy
	const float cursorBlinkTime = 0.5f; // Thời gian nhấp nháy (0.5 giây)
	int cnt = 0;

	while (GameSave.isOpen()) {
		sf::Event event;
		while (GameSave.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				GameSave.close();

			// Xử lý nhập ký tự
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode == '\b') { // Xóa ký tự
					if (!filename.empty())
						filename.pop_back();
				}
				else if (event.text.unicode == '\r') {
					if (!filename.empty()) {
						SaveGame("Assets/GameHistory/" + filename + ".txt");
						isFileSaved = true;
					}
				}
				else if (event.text.unicode < 128) { // Nhập ký tự
					filename += static_cast<char>(event.text.unicode);
				}
			}
		}

		if (cursorClock.getElapsedTime().asSeconds() >= cursorBlinkTime) {
			showCursor = !showCursor; // Đảo trạng thái
			cursorClock.restart();   // Reset thời gian
		}

		// Cập nhật text hiển thị (thêm con trỏ nếu cần)
		if (showCursor)
			filenameText.setString(filename + "|");
		else
			filenameText.setString(filename);

		// Vẽ màn hình
		GameSave.clear(sf::Color(50, 50, 50));
		GameSave.draw(inputBox);
		GameSave.draw(instruction);
		GameSave.draw(filenameText);

		if (isFileSaved) {
			sf::Text success("File saved successfully!", font, 20);
			success.setFillColor(sf::Color::Green);
			success.setPosition(200, 265);
			GameSave.draw(success);
		}
		cnt += isFileSaved;
		GameSave.display();

		if (isFileSaved && cnt == 30) {
			GameSave.close();
			return;
		}
	}
}

void Playaswhite::PrintLoadFile() {
	GameLoad.create(VideoMode(800, 600), "Save Game");
	GameLoad.setPosition(Vector2i(400, 300));
	sf::Image icon;
	icon.loadFromFile("Assets/images/load.png");
	GameLoad.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	std::vector<std::string> files;
	for (const auto& entry : fs::directory_iterator("Assets/GameHistory")) {
		if (entry.is_regular_file()) {
			files.push_back(entry.path().filename().string());
		}
	}

	std::vector<FileItem> fileButtons;
	const float buttonWidth = 400;
	const float buttonHeight = 50;
	const float buttonSpacing = 10;

	for (size_t i = 0; i < files.size(); ++i) {
		FileItem item;

		item.button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
		item.button.setFillColor(sf::Color(200, 200, 200));
		item.button.setOutlineThickness(2);
		item.button.setOutlineColor(sf::Color::Black);
		item.button.setPosition(200, i * (buttonHeight + buttonSpacing));

		item.text.setFont(font);
		item.text.setString(files[i]);
		item.text.setCharacterSize(24);
		item.text.setFillColor(sf::Color::Black);
		item.text.setPosition(item.button.getPosition().x + 10, item.button.getPosition().y + 10);

		fileButtons.push_back(item);
	}

	sf::RectangleShape scrollbar;
	scrollbar.setSize(sf::Vector2f(20, 400));
	scrollbar.setFillColor(sf::Color(150, 150, 150));
	scrollbar.setPosition(650, 100);

	sf::RectangleShape slider;
	slider.setSize(sf::Vector2f(20, 50));
	slider.setFillColor(sf::Color(100, 100, 100));
	slider.setPosition(650, 100);

	const float visibleHeight = 400;
	const float totalHeight = files.size() * (buttonHeight + buttonSpacing);

	float sliderRatio = (totalHeight > visibleHeight) ? (visibleHeight / totalHeight) : 1.0f;
	slider.setSize(sf::Vector2f(20, sliderRatio * scrollbar.getSize().y));

	float scrollOffset = 0;
	float currentScroll = 0;
	float targetScroll = 0;
	const float scrollSpeed = 0.5f;
	bool isDraggingSlider = false;
	float dragOffsetY = 0;

	sf::Clock deltaClock;

	while (GameLoad.isOpen()) {
		sf::Event event;
		while (GameLoad.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				GameLoad.close();

			if (event.type == sf::Event::MouseWheelScrolled && totalHeight > visibleHeight) {
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					float delta = event.mouseWheelScroll.delta * 20;
					targetScroll -= delta;

					if (targetScroll < 0) targetScroll = 0;
					if (targetScroll > totalHeight - visibleHeight)
						targetScroll = totalHeight - visibleHeight;
				}
			}

			sf::Vector2f mousePos(sf::Mouse::getPosition(GameLoad));
			for (auto& item : fileButtons) {
				if (item.button.getGlobalBounds().contains(mousePos)) {
					item.button.setFillColor(sf::Color(180, 180, 180));
				}
				else {
					item.button.setFillColor(sf::Color(200, 200, 200));
				}
			}

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mousePos(sf::Mouse::getPosition(GameLoad));

				for (auto& item : fileButtons) {
					if (item.button.getGlobalBounds().contains(mousePos)) {
						loadGame("Assets/GameHistory/" + item.text.getString());
						GameLoad.close();
					}
				}
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (slider.getGlobalBounds().contains(mousePos)) {
						isDraggingSlider = true;
						dragOffsetY = mousePos.y - slider.getPosition().y;
					}
				}
			}

			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					isDraggingSlider = false;
				}
			}

			if (event.type == sf::Event::MouseMoved) {
				if (isDraggingSlider) {
					float newSliderY = mousePos.y - dragOffsetY;

					if (newSliderY < scrollbar.getPosition().y)
						newSliderY = scrollbar.getPosition().y;
					if (newSliderY > scrollbar.getPosition().y + scrollbar.getSize().y - slider.getSize().y)
						newSliderY = scrollbar.getPosition().y + scrollbar.getSize().y - slider.getSize().y;

					slider.setPosition(slider.getPosition().x, newSliderY);

					float scrollPercentage = (newSliderY - scrollbar.getPosition().y) /
						(scrollbar.getSize().y - slider.getSize().y);
					targetScroll = scrollPercentage * (totalHeight - visibleHeight);
				}
			}

		}

		float deltaTime = deltaClock.restart().asSeconds();

		if (totalHeight > visibleHeight) {
			currentScroll += (targetScroll - currentScroll) * scrollSpeed;

			float sliderY = scrollbar.getPosition().y +
				(currentScroll / (totalHeight - visibleHeight)) *
				(scrollbar.getSize().y - slider.getSize().y);
			slider.setPosition(slider.getPosition().x, sliderY);
		}
		else {
			currentScroll = 0;
		}

		for (size_t i = 0; i < fileButtons.size(); ++i) {
			fileButtons[i].button.setPosition(
				200, i * (buttonHeight + buttonSpacing) - currentScroll);
			fileButtons[i].text.setPosition(
				fileButtons[i].button.getPosition().x + 10,
				fileButtons[i].button.getPosition().y + 10);
		}

		GameLoad.clear(sf::Color(50, 50, 50));

		for (const auto& item : fileButtons) {
			GameLoad.draw(item.button);
			GameLoad.draw(item.text);
		}

		if (totalHeight > visibleHeight) {
			GameLoad.draw(scrollbar);
			GameLoad.draw(slider);
		}

		GameLoad.display();
	}
}


array<int, 3>  Playaswhite::getAImove() {
	array<int, 3> Move;

	if (Easy) {
		vector<int>indicies;
		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(Black[i]);
			if (cord.x < 0 || cord.y < 0 || cord.x >= 9 || cord.y >= 9) continue;
			indicies.push_back(i);
		}

		shuffle(indicies.begin(), indicies.end(), rd);

		while (indicies.size()) {
			vector<Vector2i> valid;
			getValidMove(Black[indicies.back()], valid);
			index = indicies.back();

			for (auto move : valid) if (KingSafe(move)) {
				Move[0] = move.x;
				Move[1] = move.y;
				Move[2] = random(1, 4);
				return Move;
			}

			indicies.pop_back();
		}
	}

	if (Medium) {
		Amn.Reset(1);

		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(White[i]);
			Amn.UpdBoard(cord.y * 8 + cord.x, 0, White[i].type);
		}

		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(Black[i]);
			Amn.UpdBoard(cord.y * 8 + cord.x, 1, Black[i].type);
		}

		Amn.upd_castle(0, king_main, White[4].played, 0);
		Amn.upd_castle(1, king_main, Black[4].played, 0);

		Amn.upd_castle(0, rook_main, White[0].played, 1);
		Amn.upd_castle(0, rook_main, White[7].played, 0);

		Amn.upd_castle(1, rook_main, Black[0].played, 1);
		Amn.upd_castle(1, rook_main, Black[7].played, 0);

		Amn.MakeMove();

		array<int, 4> res = Amn.getDecision(0);

		pair<int, int> from = Square_to_cord(res[0]);
		pair<int, int> to = Square_to_cord(res[1]);

		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(Black[i]);
			if (cord.x == from.first && cord.y == from.second) {
				index = i;
				Move[0] = to.first;
				Move[1] = to.second;
				Move[2] = res[2];

				return Move;
			}
		}
	}

	if (Hard) {
		Amn.Reset(1);

		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(White[i]);
			Amn.UpdBoard(cord.y * 8 + cord.x, 0, White[i].type);
		}

		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(Black[i]);
			Amn.UpdBoard(cord.y * 8 + cord.x, 1, Black[i].type);
		}

		Amn.upd_castle(0, king_main, White[4].played, 0);
		Amn.upd_castle(1, king_main, Black[4].played, 0);

		Amn.upd_castle(0, rook_main, White[0].played, 1);
		Amn.upd_castle(0, rook_main, White[7].played, 0);

		Amn.upd_castle(1, rook_main, Black[0].played, 1);
		Amn.upd_castle(1, rook_main, Black[7].played, 0);

		Amn.MakeMove();

		array<int, 4> res = Amn.getDecision(1);

		pair<int, int> from = Square_to_cord(res[0]);
		pair<int, int> to = Square_to_cord(res[1]);

		for (int i = 0; i < 16; i++) {
			Vector2i cord = getCord(Black[i]);
			if (cord.x == from.first && cord.y == from.second) {
				index = i;
				Move[0] = to.first;
				Move[1] = to.second;
				Move[2] = res[2];

				return Move;
			}
		}
	}

	return Move;
}

int Playaswhite::handleEvents(RenderWindow& window) {
	Event event;

	if (pawnPromote) {
		while (promotion.pollEvent(event)) {
			int x = PawnPromotion(event);
			criticalMove = 0;

			if (x > 0) sound[4].play();

			if (x == 0) {
				pawnPromote = pieceSelected = 0;
				break;
			}

			if (x == -1) continue;

			if (x == rook_main) White[index].shape.setTexture(white[rook_main]), White[index].type = rook_main;
			if (x == queen_main) White[index].shape.setTexture(white[queen_main]), White[index].type = queen_main;
			if (x == bishop_main) White[index].shape.setTexture(white[bishop_main]), White[index].type = bishop_main;
			if (x == knight_main) White[index].shape.setTexture(white[knight_main]), White[index].type = knight_main;

			TakePiece(target);

			UpdHistoryWhite(White[index], target);
			White[index].shape.setPosition(target.x * SIZE, target.y * SIZE);
			White[index].played++;

			NumberOfMove++;
			UpdBoardState();
			ResetRedo();
			pieceSelected = 0;
			isWhiteTurn = 0;
			if (check()) sound[2].play();

			countState[getState()]++;
			isGameEnd = EndGame();
			pawnPromote = 0;
			promotion.close();

			for (int i = 0; i < 16; i++) {
				Blackdrag[i] = Black[i];
				Whitedrag[i] = White[i];
			}
			continue;
		}
	}

	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) window.close();
		Vector2i pos = Mouse::getPosition(window);

		UndoRedo(event, window);

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Reset.getGlobalBounds().contains(pos.x, pos.y)) {
			ResetAll();
			return Playaswhite_main;
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Quit.getGlobalBounds().contains(pos.x, pos.y)) {
			promotion.close();
			return Initial_main;
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && settingbutton.getGlobalBounds().contains(pos.x, pos.y)) {
			_settings = 1;
			continue;
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Save_Game.getGlobalBounds().contains(pos.x, pos.y)) {
			promotion.close();
			PrintSaveFile();
			continue;
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && Load_Game.getGlobalBounds().contains(pos.x, pos.y)) {
			promotion.close();
			PrintLoadFile();
			continue;
		}

		if (isGameEnd) {
			Vector2i pos = Mouse::getPosition(window);
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && playAgain.getGlobalBounds().contains(pos.x, pos.y)) {
				ResetAll();
			}

			return Playaswhite_main;
		}

		for (int i = 0; i < 16; i++) {
			Convert(Whitedrag[i].shape, pos, 170);
			Convert(Blackdrag[i].shape, pos, 170);
		}

		if (isWhiteTurn == 0) {
			array<int, 3> AI = getAImove();
			MakeMove(AI[0], AI[1], AI[2]);
			for (int i = 0; i < 16; i++) {
				Blackdrag[i] = Black[i];
				Whitedrag[i] = White[i];
			}
			return Playaswhite_main;
		}


		//if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
		//	if (pieceSelected == 0) {
		//		if (isWhiteTurn) {
		//			for (int i = 0; i < 16; i++) {
		//				if (White[i].shape.getGlobalBounds().contains(pos.x, pos.y)) {
		//					index = i;
		//					pieceSelected = 1;
		//				}
		//			}

		//			if (pieceSelected) {
		//				validMoves.clear();
		//				getValidMove(White[index], validMoves);
		//			}
		//		}
		//	}
		//	else {
		//		Vector2i temp = pos;
		//		temp.x = temp.x / SIZE;
		//		temp.y = temp.y / SIZE;

		//		MakeMove(temp.x, temp.y, 0); 
		//	}
		//}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
			if (pieceSelected == 0 && isWhiteTurn) {
				for (int i = 0; i < 16; i++) {
					if (White[i].shape.getGlobalBounds().contains(pos.x, pos.y)) {
						index = i;
						pieceSelected = 1;
						isDragging = 1;
						originalPosition = White[i].shape.getPosition();
					}
				}

				if (pieceSelected) {
					validMoves.clear();
					getValidMove(White[index], validMoves);
				}
			}
		}

		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			if (isDragging) {
				sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
				pos.x = pos.x / SIZE;
				pos.y = pos.y / SIZE;

				if (MakeMove(pos.x, pos.y, 0) == 0)
					Whitedrag[index].shape.setPosition(originalPosition);
				else {
					for (int i = 0; i < 16; i++) {
						Blackdrag[i] = Black[i];
						Whitedrag[i] = White[i];
					}
				}

				isDragging = false;
			}
		}

		if (event.type == sf::Event::MouseMoved && isDragging) {
			Whitedrag[index].shape.setPosition(event.mouseMove.x - SIZE / 2, event.mouseMove.y - SIZE / 2);
		}
	}
	return Playaswhite_main;
}

void Playaswhite::BoardSetUp(string path, char numberBoard) {
	string BoardPath = "Assets/images/BoardGame/board";
	BoardPath += numberBoard;
	BoardPath += ".png";

	BoardTexture.loadFromFile(BoardPath);
	Board.setTexture(BoardTexture);


	int cnt = 0;
	string tmp = path;

	tmp += "white-rook.png";
	white[rook_main].loadFromFile(tmp);
	White[0] = Figure(rook_main, white[rook_main]);
	White[7] = Figure(rook_main, white[rook_main]);

	tmp = path; tmp += "white-knight.png";
	white[knight_main].loadFromFile(tmp);

	White[1] = Figure(knight_main, white[knight_main]);
	White[6] = Figure(knight_main, white[knight_main]);

	tmp = path; tmp += "white-bishop.png";
	white[bishop_main].loadFromFile(tmp);
	White[2] = Figure(bishop_main, white[bishop_main]);
	White[5] = Figure(bishop_main, white[bishop_main]);

	tmp = path; tmp += "white-queen.png";
	white[queen_main].loadFromFile(tmp);
	White[3] = Figure(queen_main, white[queen_main]);

	tmp = path; tmp += "white-king.png";
	white[king_main].loadFromFile(tmp);
	White[4] = Figure(king_main, white[king_main]);

	tmp = path;  tmp += "black-rook.png";
	black[rook_main].loadFromFile(tmp);
	Black[0] = Figure(rook_main, black[rook_main]);
	Black[7] = Figure(rook_main, black[rook_main]);

	tmp = path; tmp += "black-knight.png";
	black[knight_main].loadFromFile(tmp);
	Black[1] = Figure(knight_main, black[knight_main]);
	Black[6] = Figure(knight_main, black[knight_main]);

	tmp = path; tmp += "black-bishop.png";
	black[bishop_main].loadFromFile(tmp);
	Black[2] = Figure(bishop_main, black[bishop_main]);
	Black[5] = Figure(bishop_main, black[bishop_main]);

	tmp = path; tmp += "black-queen.png";
	black[queen_main].loadFromFile(tmp);
	Black[3] = Figure(queen_main, black[queen_main]);

	Test.loadFromFile(tmp);
	t.setTexture(Test);
	t.setPosition(0, SIZE);

	tmp = path; tmp += "black-king.png";
	black[king_main].loadFromFile(tmp);
	Black[4] = Figure(king_main, black[king_main]);

	for (int i = 8; i < 16; i++) {
		tmp = path; tmp += "white-pawn.png";
		white[5].loadFromFile(tmp);
		White[i] = Figure(pawn_main, white[5]);
		tmp = path; tmp += "black-pawn.png";
		black[5].loadFromFile(tmp);
		Black[i] = Figure(pawn_main, black[5]);
	}

	for (int i = 0; i < 16; i++) {
		if (i < 8) White[i].SetPosition(cnt * SIZE, 7 * SIZE);
		else White[i].SetPosition(cnt * SIZE, 6 * SIZE);

		if (i < 8) Black[i].SetPosition(cnt * SIZE, 0);
		else Black[i].SetPosition(cnt * SIZE, SIZE);

		cnt++;
		if (cnt == 8) cnt = 0;
	}

	NumberOfMove = 0;
	countState.clear();
	isWhiteTurn = 1;
	isGameEnd = 0;
	square_from = { -1000, -1000 };
	square_to = { -1000, -1000 };

	UpdBoardState();
	for (int i = 0; i < 16; i++) {
		Blackdrag[i] = Black[i];
		Whitedrag[i] = White[i];
	}
}

void Playaswhite::UpdChanges(string path, char numberBoard) {
	string BoardPath = "Assets/images/BoardGame/rboard";
	BoardPath += numberBoard;
	BoardPath += ".png";

	BoardTexture.loadFromFile(BoardPath);
	Board.setTexture(BoardTexture);

	int cnt = 0;
	string tmp = path;

	tmp += "white-rook.png";
	white[rook_main].loadFromFile(tmp);
	White[0] = Figure(rook_main, white[rook_main]);
	White[7] = Figure(rook_main, white[rook_main]);

	tmp = path; tmp += "white-knight.png";
	white[knight_main].loadFromFile(tmp);

	White[1] = Figure(knight_main, white[knight_main]);
	White[6] = Figure(knight_main, white[knight_main]);

	tmp = path; tmp += "white-bishop.png";
	white[bishop_main].loadFromFile(tmp);
	White[2] = Figure(bishop_main, white[bishop_main]);
	White[5] = Figure(bishop_main, white[bishop_main]);

	tmp = path; tmp += "white-queen.png";
	white[queen_main].loadFromFile(tmp);
	White[3] = Figure(queen_main, white[queen_main]);

	tmp = path; tmp += "white-king.png";
	white[king_main].loadFromFile(tmp);
	White[4] = Figure(king_main, white[king_main]);

	tmp = path;  tmp += "black-rook.png";
	black[rook_main].loadFromFile(tmp);
	Black[0] = Figure(rook_main, black[rook_main]);
	Black[7] = Figure(rook_main, black[rook_main]);

	tmp = path; tmp += "black-knight.png";
	black[knight_main].loadFromFile(tmp);
	Black[1] = Figure(knight_main, black[knight_main]);
	Black[6] = Figure(knight_main, black[knight_main]);

	tmp = path; tmp += "black-bishop.png";
	black[bishop_main].loadFromFile(tmp);
	Black[2] = Figure(bishop_main, black[bishop_main]);
	Black[5] = Figure(bishop_main, black[bishop_main]);

	tmp = path; tmp += "black-queen.png";
	black[queen_main].loadFromFile(tmp);
	Black[3] = Figure(queen_main, black[queen_main]);

	Test.loadFromFile(tmp);
	t.setTexture(Test);
	t.setPosition(0, SIZE);

	tmp = path; tmp += "black-king.png";
	black[king_main].loadFromFile(tmp);
	Black[4] = Figure(king_main, black[king_main]);

	for (int i = 8; i < 16; i++) {
		tmp = path; tmp += "white-pawn.png";
		white[5].loadFromFile(tmp);
		White[i] = Figure(pawn_main, white[5]);
		tmp = path; tmp += "black-pawn.png";
		black[5].loadFromFile(tmp);
		Black[i] = Figure(pawn_main, black[5]);
	}

	for (int i = 0; i < 16; i++) {
		if (i < 8) White[i].SetPosition((7 - cnt) * SIZE, 0);
		else White[i].SetPosition((7 - cnt) * SIZE, SIZE);

		if (i < 8) Black[i].SetPosition((7 - cnt) * SIZE, 7 * SIZE);
		else Black[i].SetPosition((7 - cnt) * SIZE, 6 * SIZE);

		cnt++;
		if (cnt == 8) cnt = 0;
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i];
		White[i].type = lst_move[2];
		White[i].shape.setTexture(white[lst_move[2]]);
		White[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		White[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		array<int, 4> lst_move = SaveBoard[NumberOfMove][i + 16];
		Black[i].type = lst_move[2];
		Black[i].shape.setTexture(black[lst_move[2]]);
		Black[i].SetPosition(lst_move[0] * SIZE, lst_move[1] * SIZE);
		Black[i].played = lst_move[3];
	}

	for (int i = 0; i < 16; i++) {
		Blackdrag[i] = Black[i];
		Whitedrag[i] = White[i];
	}
}

void Playaswhite::draw(sf::RenderWindow& window) {
	Texture background;
	background.loadFromFile("Assets/images/mountain.jpg");
	Sprite backg(background);

	window.draw(backg);
	window.draw(Board);

	if (NumberOfMove > 0 && pawnPromote == 0) {
		RectangleShape from, to;
		from.setSize(Vector2f(75.4, 75.4));
		from.setFillColor(sf::Color(245, 246, 130, 255));
		from.setPosition(square_from.x * SIZE * 1.0 - 0.47, square_from.y * SIZE * 1.0 - 0.5);

		to.setSize(Vector2f(75.0, 75.0));
		to.setFillColor(sf::Color(245, 246, 130, 255));
		to.setPosition(square_to.x * SIZE * 1.0 - 0.47, square_to.y * SIZE * 1.0 - 0.5);
		window.draw(from);
		window.draw(to);
	}

	for (int i = 0; i < 16; i++) {
		window.draw(Blackdrag[i].shape);
		window.draw(Whitedrag[i].shape);
	}

	if (pieceSelected && isWhiteTurn) {
		drawValidMoves(window, validMoves);
	}

	if (pawnPromote)
		drawPawnPromote();
	else promotion.close();

	drawUndoRedo(window);
	sf::Text quitButtonText, SaveGameText, LoadGameText;

	Quit.setPosition(600, 380);
	Save_Game.setPosition(600, 330);
	Load_Game.setPosition(600, 280);

	SaveGameText.setFont(font);
	SaveGameText.setString("Save");
	SaveGameText.setCharacterSize(30);
	SaveGameText.setFillColor(sf::Color::White);

	LoadGameText.setFont(font);
	LoadGameText.setString("Load Game");
	LoadGameText.setCharacterSize(28);
	LoadGameText.setFillColor(sf::Color::White);

	LoadGameText.setPosition(
		Load_Game.getPosition().x + (Load_Game.getSize().x - LoadGameText.getLocalBounds().width) / 2,
		Load_Game.getPosition().y + (Load_Game.getSize().y - LoadGameText.getLocalBounds().height) / 2 - 5
	);

	SaveGameText.setPosition(
		Save_Game.getPosition().x + (Save_Game.getSize().x - SaveGameText.getLocalBounds().width) / 2,
		Save_Game.getPosition().y + (Save_Game.getSize().y - SaveGameText.getLocalBounds().height) / 2 - 5
	);

	quitButtonText.setFont(font);
	quitButtonText.setString("Quit");
	quitButtonText.setCharacterSize(30);
	quitButtonText.setFillColor(sf::Color::White);
	quitButtonText.setPosition(
		Quit.getPosition().x + (Quit.getSize().x - quitButtonText.getLocalBounds().width) / 2,
		Quit.getPosition().y + (Quit.getSize().y - quitButtonText.getLocalBounds().height) / 2 - 5
	);

	Vector2i pos = Mouse::getPosition(window);

	if (Save_Game.getGlobalBounds().contains(pos.x, pos.y)) SaveGameText.setStyle(Text::Bold);
	else SaveGameText.setStyle(Text::Regular);

	if (Load_Game.getGlobalBounds().contains(pos.x, pos.y)) LoadGameText.setStyle(Text::Bold);
	else LoadGameText.setStyle(Text::Regular);

	if (Quit.getGlobalBounds().contains(pos.x, pos.y)) quitButtonText.setStyle(Text::Bold);
	else quitButtonText.setStyle(Text::Regular);

	Convert(settingbutton, pos);

	Text Indicator;

	Indicator.setFont(font);
	Indicator.setCharacterSize(30);
	Indicator.setPosition(620, 34);

	if (isWhiteTurn) {
		Indicator.setString("White's Turn");
		Indicator.setFillColor(sf::Color::White);
		Indicator.setOutlineColor(sf::Color::Black);
		Indicator.setOutlineThickness(2);

	}
	else {
		Indicator.setString("Black's Turn");
		Indicator.setFillColor(sf::Color::Black);
		Indicator.setOutlineColor(sf::Color::White);
		Indicator.setOutlineThickness(2);
	}

	window.draw(Indicator);

	window.draw(Quit);
	window.draw(quitButtonText);
	window.draw(Save_Game);
	window.draw(SaveGameText);
	window.draw(settingbutton);
	window.draw(Load_Game);
	window.draw(LoadGameText);

	if (isGameEnd) {
		Quit.setSize(sf::Vector2f(200, 50));
		Quit.setFillColor(sf::Color::Red);
		Quit.setPosition(300, 360);

		sf::Text endText;
		endText.setFont(font);
		endText.setCharacterSize(40);


		if (isGameEnd == 1) {
			if (isWhiteTurn) endText.setString("Bot Wins!");
			if (isWhiteTurn == 0) endText.setString("You Win!");
		}
		else {
			if (isGameEnd == stalemate_main)
				endText.setString("Stalemate! Draw!");
			else if (isGameEnd == insufficient_main)
				endText.setString("Insufficient material! Draw!");
			else if (isGameEnd == fifty_main)
				endText.setString("50-Move! Draw!");
			else
				endText.setString("Repetition! Draw!");
		}

		sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
		overlay.setFillColor(sf::Color(0, 0, 0, 150));

		endText.setFillColor(sf::Color::White);
		endText.setPosition(window.getSize().x / 2 - endText.getLocalBounds().width / 2,
			window.getSize().y / 2 - endText.getLocalBounds().height / 2);

		sf::Text playAgainButtonText;
		playAgainButtonText.setFont(font);
		playAgainButtonText.setString("Play Again");
		playAgainButtonText.setCharacterSize(30);
		playAgainButtonText.setFillColor(sf::Color::White);

		playAgainButtonText.setPosition(
			playAgain.getPosition().x + (playAgain.getSize().x - playAgainButtonText.getLocalBounds().width) / 2,
			playAgain.getPosition().y + (playAgain.getSize().y - playAgainButtonText.getLocalBounds().height) / 2 - 5
		);

		quitButtonText.setPosition(
			Quit.getPosition().x + (Quit.getSize().x - quitButtonText.getLocalBounds().width) / 2,
			Quit.getPosition().y + (Quit.getSize().y - quitButtonText.getLocalBounds().height) / 2 - 5
		);

		Vector2i pos = Mouse::getPosition(window);

		if (playAgain.getGlobalBounds().contains(pos.x, pos.y)) playAgainButtonText.setStyle(Text::Bold);
		else playAgainButtonText.setStyle(Text::Regular);

		if (Quit.getGlobalBounds().contains(pos.x, pos.y)) quitButtonText.setStyle(Text::Bold);
		else quitButtonText.setStyle(Text::Regular);

		window.draw(overlay);
		window.draw(endText);
		window.draw(playAgain);
		window.draw(playAgainButtonText);
		window.draw(Quit);
		window.draw(quitButtonText);
	}
}