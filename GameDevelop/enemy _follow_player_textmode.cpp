
 // 2D direction
 // Pos -> player
// enemy -> enemy

    if (abs(pos.X - enemy.X) > abs(pos.Y - enemy.Y)) {
				if (pos.X > enemy.X) enemy.X++;
				else enemy.X--;

		}
		else {
				if (pos.Y > enemy.Y) enemy.Y++;
				else enemy.Y--;
		}


// 3D direction 
// Pos -> player
// enemy -> enemy

   if (pos.X - enemy.X > pos.Y - enemy.Y) {
			if (pos.X > enemy.X) enemy.X++;
			else enemy.X--;
	 }
	else {
			if (pos.Y > enemy.Y) enemy.Y++;
			else enemy.Y--;
	}


	if (enemy.X - pos.X > enemy.Y - pos.Y) {
			if (pos.X < enemy.X) enemy.X--;
			else enemy.X++;
	}
	else {
			if (pos.Y < enemy.Y) enemy.Y--;
			else enemy.Y++;
	}	
		
