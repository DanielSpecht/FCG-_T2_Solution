//FCG
//Aluno - Daniel Specht Silva Menezes
//Professor - Marcelo Gattass
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <string.h>
#include "dirent.h"
#include <gl\glut.h>
#include "CornerTable.h"
#include <time.h>
using namespace std;
#pragma warning(disable:4996)//Para usar scanf, especialmente fora da main

#pragma region Cabeçalhos de função

int FindTriangleThatContainsPoint(int corner, int cornerPrevious);
void pointGenerate(double &x, double &y);

void Inicializa(void);
void EspecificaParametrosVisualizacao(void);
void AlteraTamanhoJanela(GLsizei w, GLsizei h);
void GerenciaMouse(int button, int state, int x, int y);

void DesenhaTriangulos();
void DesenhaPonto();
void Desenha();

char* SelectFileFromDirectory(char* directory, int directoryLenght);
CornerTable GenerateCornerTableFromFile(char* FilePath);

#pragma endregion

#pragma region Variáveis globais

//Valores de atributos da câmera
GLfloat angle, fAspect;
//Se já encontrou o triângulo
int encontrou = 0;
//Pontos escolhidos
double _X, _Y; 
//Corner sendo percorrido
int _cornerAtual = 0;
//CornerTable sendo utilizada
CornerTable *_cornerTable;

#pragma endregion

#pragma region Manutenção da Janela e Mouse

// Inicializa parâmetros de rendering
void Inicializa(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	angle = 45;
}
// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);

	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva, field of view
	gluPerspective(angle, fAspect, 0.5, 500);

	// Especifica sistema de coordenadas do modelo
	// Todas as futuras alterações, tais como operações de escala, rotação e translação, irão afetar os modelos da cena
	glMatrixMode(GL_MODELVIEW);

	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

	/**
	gluLookAt( GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz );

	eyeX, eyeY, eyeZ
	Specifies the position of the eye point.

	centerX, centerY, centerZ
	Specifies the position of the reference point.

	upX, upY, upZ
	Specifies the direction of the up vector.
	**/
}
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;


	glViewport(0, 0, w, h);
	/**
	Define:
	(x,y) A área dentro janela, em coordenadas de tela, que OpenGL pode usar para fazer o desenho.
	(w,h) LArgura e altura da janela
	**/

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w / (GLfloat)h;


	EspecificaParametrosVisualizacao();
}
// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (angle >= 10) angle -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (angle <= 130) angle += 5;
		}
	EspecificaParametrosVisualizacao();
	glFlush();
}

#pragma endregion 

#pragma region Funções de desenho de triângulo e chamada do caso base 

void DesenhaTriangulos()
{
	int cont;
	int nTriangulos = _cornerTable->getNumTriangles();
	double *points = _cornerTable->getAttributes();
	const CornerType *V = _cornerTable->getTriangleList();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);

	//Imprime os Triângulos
	for (cont = 0; cont < nTriangulos; cont++)
	{
		int trianguloAtual = _cornerTable->cornerTriangle(_cornerAtual);
		int v1, v2, v3;
		//Número dos vértices do triângulo
		v1 = V[cont * 3];
		v2 = V[(cont * 3) + 1];
		v3 = V[(cont * 3) + 2];

		glColor3f(0, 0, 1);

		//Obtém os pontos do triângulo e o desenha na janela
		//p1
		glVertex3d(points[v1 * 3], points[(v1 * 3) + 1], points[(v1 * 3) + 2]);
		//p2
		glVertex3d(points[v2 * 3], points[(v2 * 3) + 1], points[(v2 * 3) + 2]);
		//p3
		glVertex3d(points[v3 * 3], points[(v3 * 3) + 1], points[(v3 * 3) + 2]);
	}

	//Pinta o Triângulo Sendo percorrido de vermelho
	{
		int v1, v2, v3;
		cont = _cornerTable->cornerTriangle(_cornerAtual);
		
		//Número dos vértices do triângulo sendo percorrido
		v1 = V[cont * 3];
		v2 = V[(cont * 3) + 1];
		v3 = V[(cont * 3) + 2];

		//Obtém os pontos do triângulo sendo tratado e o desenha na janela
		glColor3f(1, 0, 0);
		//p1
		glVertex3d(points[v1 * 3], points[(v1 * 3) + 1], points[(v1 * 3) + 2]);
		//p2
		glVertex3d(points[v2 * 3], points[(v2 * 3) + 1], points[(v2 * 3) + 2]);
		//p3
		glVertex3d(points[v3 * 3], points[(v3 * 3) + 1], points[(v3 * 3) + 2]);
	}

	glEnd();

}

void DesenhaPonto()
{
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 1);
	glVertex3d(_X, _Y, 0);
	glEnd();
}

void Desenha()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Se encontrou o triângulo, desconsidera a função desenha.
	if (!encontrou)
	{
		FindTriangleThatContainsPoint(0, -1);
		encontrou = 1;
	}
}

#pragma endregion 

#pragma region Funções de leitura de arquivos e montagem da CornerTable

void pointGenerate(double &x, double &y)
{
	double a1;
	double a2;
	double a3;
	double a4;
	double a5;
	double a6;

	srand(time(NULL));
	a1 = ((rand() % 1000001) / 1000000.0) / 5.0;
	a2 = ((rand() % 1000001) / 1000000.0) / 5.0;
	a3 = ((rand() % 1000001) / 1000000.0) / 5.0;
	a4 = ((rand() % 1000001) / 1000000.0) / 5.0;
	a5 = ((rand() % 1000001) / 1000000.0) / 5.0;
	a6 = 1.0 - (a1 + a2 + a3 + a4 + a5);

	x = a1 * (2) + a2 * (1) + a3 * (-1) + a4 * (-2) + a5 * (-1) + a6 * (1);
	y = a1 * (0) + a2 * (1.73205) + a3 * (1.73205) + a4 * (0) + a5 * (-1.73205) + a6 * (-1.73205);
}

char* SelectFileFromDirectory(char* directory, int directoryLenght)
{
	DIR *d;
	struct dirent *dir;
	int cont = 0;
	int selected;
	char *returnValue;

	printf("Selecione o numero do arquivo Desejado:\n");

	d = opendir(directory);
	if (d)
	{
		dir = readdir(d);//pula .
		dir = readdir(d);//pula ..
		while ((dir = readdir(d)) != NULL)
		{
			cont++;
			if (cont <= 9)
			{
				printf(" ");
			}
			printf("%d - %s\n", cont, dir->d_name);
		}

		rewinddir(d);
		dir = readdir(d);//pula .
		dir = readdir(d);//pula ..

		scanf_s("%d", &selected);

		for (cont = 0; cont < selected; cont++)
		{
			dir = readdir(d);
		}
		//printf("Selecionado : %s \n", dir->d_name);

		returnValue = (char*)malloc(directoryLenght + dir->d_namlen + 1 + 1);//+1 para o '/' e +1 para \0

		for (cont = 0; cont < directoryLenght; cont++)
		{
			returnValue[cont] = directory[cont];
		}
		returnValue[directoryLenght] = '/';

		for (cont = 0; cont < dir->d_namlen; cont++)
		{

			returnValue[directoryLenght + 1 + cont] = dir->d_name[cont];
		}
		returnValue[directoryLenght + 1 + cont] = '\0';//Término da string

		//printf("\n\n\n>>>%s", returnValue);
		return returnValue;
	}

	fprintf(stderr, "Can't open input file in.list!\n");
}

CornerTable GenerateCornerTableFromFile(char* FilePath)
{
	FILE *fp = fopen(FilePath, "r");
	CornerType nPoints, nTriangles, cont;
	double *points;
	CornerType *vertices;

	if (fp == NULL) {
		fprintf(stderr, "Can't open input file in.list!\n");
	}

	fscanf(fp, "%s");//Le a linha END
	fscanf(fp, "%d %d %d", &nPoints, &nTriangles);

	//Aloca memória
	points = (double*)malloc(3 * nPoints*sizeof(double));
	vertices = (CornerType*)malloc(3 * nTriangles*sizeof(CornerType));

	//Obtém pontos dos vértices
	for (cont = 0; cont < nPoints; cont++)
	{
		float x, y, z;
		fscanf(fp, "%f %f %f", &x, &y, &z);
		points[(cont * 3)] = x;
		points[(cont * 3) + 1] = y;
		points[(cont * 3) + 2] = z;
	}

	//Obtém vertices dos triângulos
	for (cont = 0; cont < nTriangles; cont++)
	{
		int n, v1, v2, v3;
		fscanf(fp, "%d %d %d %d", &n, &v1, &v2, &v3);
		vertices[(cont * 3)] = v1;
		vertices[(cont * 3) + 1] = v2;
		vertices[(cont * 3) + 2] = v3;
	}
	return CornerTable(vertices, points, nTriangles, nTriangles, 3);
}

#pragma endregion

int FindTriangleThatContainsPoint(int corner, int cornerPrevious)
{
	double x1, x2, x3, y1, y2, y3;
	int vertexIndex1, vertexIndex2, vertexIndex3;
	double u, v, w;
	double *att = _cornerTable->getAttributes();
	int trianguloAtual = _cornerTable->cornerTriangle(corner);
	unsigned int retTime = time(0) + 1;   // Get finishing time

	_cornerAtual = corner;

	DesenhaTriangulos();
	DesenhaPonto();
	glutSwapBuffers();

	while (time(0) < retTime);

	vertexIndex1 = _cornerTable->cornerToVertexIndex(corner);
	vertexIndex2 = _cornerTable->cornerToVertexIndex(_cornerTable->cornerNext(corner));
	vertexIndex3 = _cornerTable->cornerToVertexIndex(_cornerTable->cornerPrevious(corner));

	x1 = att[vertexIndex1 * 3];
	y1 = att[(vertexIndex1 * 3) + 1];

	x2 = att[vertexIndex2 * 3];
	y2 = att[(vertexIndex2 * 3) + 1];

	x3 = att[vertexIndex3 * 3];
	y3 = att[(vertexIndex3 * 3) + 1];
	printf("%d ", trianguloAtual);
	//printf("\nTriangulo:%d\n", trianguloAtual);
	//printf("\nPontos do triangulo:(%f,%f) (%f,%f) (%f,%f)\n", x1, y1, x2, y2, x3, y3);
	//printf("Vertices do triangulo:( %d, %d, %d)\n", vertexIndex1, vertexIndex2, vertexIndex3);
	
	//Cálculo das coordenadas baricêntricas do ponto em relação ao triângulo sendo percorrido
	if ((((y2 - y3)*(x1 - x3)) + ((x3 - x2)*(y1 - y3))) == 0)//nao dividir por 0 e nao quebrar o algoritmo
	{
		u = 0;
		v = 0;
		w = 0;
	}
	else
	{
		u = (((y2 - y3)*(_X - x3)) + ((x3 - x2)*(_Y - y3))) / (((y2 - y3)*(x1 - x3)) + ((x3 - x2)*(y1 - y3)));
		v = (((y3 - y1)*(_X - x3)) + ((x1 - x3)*(_Y - y3))) / (((y2 - y3)*(x1 - x3)) + ((x3 - x2)*(y1 - y3)));
		w = 1 - u - v;
	}

	//printf("u,v,w : (%f,%f,%f)\n", u, v, w);

	//Checa se está dentro do triângulo
	//Nota: se estiver na borda, conta.
	//O único jeito das coordenadas serem positivas é se e somente se:
	if ((u >= 0) && (u <= 1) && (v >= 0) && (v <= 1) && (w >= 0) && (w <= 1))
	{
		return trianguloAtual;
	}

	if((v<=0)&&(v<w)&&(v<u) )
	{
		int nextCornerToTest = _cornerTable->cornerOpposite(_cornerTable->cornerNext(corner));

		if ((nextCornerToTest == -1) || (nextCornerToTest == cornerPrevious))//Evita loop e fazer operação "oposite inválida"
		{
			nextCornerToTest = _cornerTable->cornerOpposite(corner);//Testa o oposite

			if ((nextCornerToTest != -1) && (nextCornerToTest != cornerPrevious))//Evita loop e fazer operação "oposite inválida"
			{
				return FindTriangleThatContainsPoint(nextCornerToTest, corner);
			}
			else
			{
				nextCornerToTest = _cornerTable->cornerOpposite(_cornerTable->cornerPrevious(corner)); //Envia o previous
				return FindTriangleThatContainsPoint(nextCornerToTest, corner);
			}
		}

		return FindTriangleThatContainsPoint(nextCornerToTest, corner);
	}
	if((w<=0)&&(w<u) )
	{
		int nextCornerToTest = _cornerTable->cornerOpposite(_cornerTable->cornerPrevious(corner));

		if ((nextCornerToTest == -1) || (nextCornerToTest == cornerPrevious))//Evita loop e fazer operação "oposite inválida"
		{
			nextCornerToTest = _cornerTable->cornerOpposite(corner);//Testa o oposite

			if ((nextCornerToTest != -1) && (nextCornerToTest != cornerPrevious))//Evita loop e fazer operação "oposite inválida"
			{
				return FindTriangleThatContainsPoint(nextCornerToTest, corner);
			}
			else
			{
				nextCornerToTest = _cornerTable->cornerOpposite(_cornerTable->cornerNext(corner)); //Envia o next
				return FindTriangleThatContainsPoint(nextCornerToTest, corner);
			}
		}

		return FindTriangleThatContainsPoint(nextCornerToTest, corner);
	}
	if(u<=0)
	{
		int nextCornerToTest = _cornerTable->cornerOpposite(corner);
		
		if ((nextCornerToTest == -1) || (nextCornerToTest == cornerPrevious))//Evita loop e fazer operação "oposite inválida"
		{
			nextCornerToTest = _cornerTable->cornerOpposite(_cornerTable->cornerNext(corner));//Testa o next
			
			if ((nextCornerToTest != -1) && (nextCornerToTest != cornerPrevious))//Evita loop e fazer operação "oposite inválida"
			{
				return FindTriangleThatContainsPoint(nextCornerToTest, corner);
			}
			else
			{
				nextCornerToTest = _cornerTable->cornerOpposite(_cornerTable->cornerPrevious(corner)); //Envia o previous
				return FindTriangleThatContainsPoint(nextCornerToTest, corner);
			}
		}
		
		return FindTriangleThatContainsPoint(nextCornerToTest, corner);
	}
}

void main(int argc, char *argv[])
{
	char *filePath = SelectFileFromDirectory("./Meshes", 8);
	CornerTable a = GenerateCornerTableFromFile(filePath);
	_cornerTable = &a;
	pointGenerate(_X, _Y);
	//_cornerTable->printTriangleList();

	printf("\nPontos gerados %f,%f\n", _X, _Y);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	/**
	Avisa a GLUT que tipo de modo de exibição deve ser usado
	Single-buffered (GLUT_SINGLE)
	Todos os comandos de desenho são feitos na janela de exibição
	Double-duffered (GLUT_DOUBLE)
	GLUT_DOUBLE. Isto faz com que todo o rendering seja feito em um offscreen buffer
	Modo de cores RGBA (GLUT_RGB)
	**/
	glutInitWindowSize(700, 700);
	//Determina qual o tamanho da janela
	glutInitWindowPosition(10, 10);
	//Determina a posição da janela na tela
	glutCreateWindow("CornerTable - Search");
	glutDisplayFunc(Desenha);
	//Estabelece o callback de exibição, chamada sempre que a janela precisar ser redesenhada, inclusive em casos de redimensionamento de tela
	glutReshapeFunc(AlteraTamanhoJanela);
	//Estabelece o Callback de redimensionamento, inicia/reinicia o sistema de coordenadas
	glutMouseFunc(GerenciaMouse);
	//Estabelece a função de callback para gerenciar os eventos do mouse
	Inicializa();
	//Inicializa alguns parâmetros de rendering
	glutMainLoop();
	//Inicializa a execução da “máquina de estados” até que o programa finalize
}
