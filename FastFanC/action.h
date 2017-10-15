

// �������� �������������
void CollisionUnit() {

	register uint32 i, j;
	float dis;
	float vX, vY;	// ������
	float nvX, nvY;	// ������������� ������
	float l;		// ���������� ��������

	for (i = 0; i < SIZE_UNIT; i++) {
		for (j = i + 1; j < SIZE_UNIT; j++) {

			if (units[i].id == -1 || units[j].id == -1)
				continue;


			// ������� ��������� ����� ���������
			dis = sqrtf((units[i].x - units[j].x)*(units[i].x - units[j].x) + (units[i].y - units[j].y)*(units[i].y - units[j].y));

			if (dis == 0.0f) {
				units[i].y += units[i].r + units[j].r;
				units[i].refn = 1;
				continue;
			}

			// ���������� �������������
			if (dis < units[i].r + units[j].r) {


				// �������� 14.21 15.10.17

				// ������ ��� �������
				if (units[i].model < 5 && units[j].model < 5) {
					if (units[i].model < units[j].model) {

						//  ���������� ����������
						units[i].refn = 1;
						units[i].x = randomFloat(-180.0f, 180.0f);
						units[i].y = randomFloat(-90.0f, 90.0f);
						if (units[i].model == 0)
							units[i].model = 0;
						else
							units[i].model--;

						//  ++ ���� ���� ��� ����


						units[j].score++;
						units[j].exp++;
						// Next lvl
						if (units[j].exp > 3) {
							units[j].refn = 1;
							if (units[j].model == 4)
								units[j].model = 4;
							else
								units[j].model++;
							units[j].exp = 0;
						}
						continue;
					}
					if (units[j].model < units[i].model) {

						//  ���������� ����������
						units[j].refn = 1;
						units[j].x = randomFloat(-180.0f, 180.0f);
						units[j].y = randomFloat(-90.0f, 90.0f);
						if (units[j].model == 0)
							units[j].model = 0;
						else
							units[j].model--;

						//  ++ ���� ���� ��� ����


						units[i].score++;
						units[i].exp++;
						// Next lvl
						if (units[i].exp > 3) {
							units[i].refn = 1;
							if (units[i].model == 5)
								units[i].model = 5;
							else
								units[i].model++;
							units[i].exp = 0;
						}
						continue;
					}
					
					
				}
				
				// ������ ������ ��������
				if (units[i].model < 5 && (units[j].model == 5 || units[j].model == 6)) {
					units[i].score++;
					units[i].exp++;
					// Next lvl
					if (units[i].exp > 3) {
						units[i].refn = 1;
						if (units[i].model == 4)
							units[i].model = 4;
						else
							units[i].model++;
						units[i].exp = 0;
					}

					units[j].refn = 1;
					units[j].x = randomFloat(-180.0f, 180.0f);
					units[j].y = randomFloat(-90.0f, 90.0f);
					continue;
				}


				if (units[j].model < 5 && (units[i].model == 5 || units[i].model == 6)) {
					units[j].score++;
					units[j].exp++;
					// Next lvl
					if (units[j].exp > 3) {
						units[j].refn = 1;
						if (units[j].model == 4)
							units[j].model = 4;
						else
							units[j].model++;
						units[j].exp = 0;
					}

					units[i].refn = 1;
					units[i].x = randomFloat(-180.0f, 180.0f);
					units[i].y = randomFloat(-90.0f, 90.0f);
					continue;
				}

				// ������ ������������

				// ������ ������ ���������� ��������
				l = ((units[i].r + units[j].r) - dis) / 2.0f;
				// ������ ������ �� i-�� ������ � j-���
				vX = units[j].x - units[i].x;
				vY = units[j].y - units[i].y;

				// ��������� ������
				nvX = vX / dis;
				nvY = vY / dis;

				// �������� j-�� ������ �� ���������� l
				units[j].x += nvX * l;
				units[j].y += nvY * l;

				// �������� i-�� ������ �� ���������� l � �������� �������
				units[i].x -= nvX * l;
				units[i].y -= nvY * l;

				// �������� �� ����������
				units[i].refn = 1;
				units[j].refn = 1;
			}

		}
	}
}


void Action() {

	register uint32 i;

	for (i = 0; i < SIZE_UNIT; i++) {
		if (units[i].id == -1)
			continue;

		if (units[i].active == 1) {

			if (units[i].model < 5) {
				units[i].x -= sinf(units[i].a) * PIG_SPEED;
				units[i].y += cosf(units[i].a) * PIG_SPEED;

				units[i].z += sinf(units[i].dz*1.5f) / 2;
				units[i].dz++;

				if (units[i].dz > 1000.0f)
					units[i].dz = 0.0;

				units[i].refn = 1;
				continue;
			}

			
		}

	}

}


void GameArea() {
	for (int i = SIZE_PLAYER; i--;) {
		if (units[i].x < -MAP_WIDTH)
			units[i].x = -MAP_WIDTH;

		if (units[i].x > MAP_WIDTH)
			units[i].x = MAP_WIDTH;

		if (units[i].y < -MAP_HEIGHT)
			units[i].y = -MAP_HEIGHT;

		if (units[i].y > MAP_HEIGHT)
			units[i].y = MAP_HEIGHT;
	}
}