//Alunos: Lucas Schneider Ludwig, Sandro Frizon Junior
#include <stdio.h>
#include <stdlib.h>

struct matrix {
	struct matrix* right;
	struct matrix* below;
	int line;
	int column;
	float info;
};
typedef struct matrix Matrix;

Matrix* matrix_create ( void );
void matrix_destroy ( Matrix* m );
void matrix_print ( Matrix* m );
Matrix* matrix_add ( Matrix* m, Matrix* n );
Matrix* matrix_multiply ( Matrix* m, Matrix* n );
Matrix* matrix_transpose ( Matrix* m );
float matrix_getelem ( Matrix* m, int x, int y );
void pointMatrix ( Matrix* result, Matrix* newMatNode );
void matrix_setelem ( Matrix* m, int x, int y, float elem );

int main() {
    Matrix* A = matrix_create();
    matrix_print ( A );
    Matrix* B = matrix_create();
    matrix_print ( B );
    Matrix* C =  matrix_add ( A, B );
    matrix_print ( C );
    matrix_destroy( C );

    return 0;
}

Matrix* matrix_create( void ) {
    Matrix* mat = (Matrix *)malloc(sizeof(Matrix));
    mat->line = -1;
    mat->column = -1;
    mat->right = mat;
    mat->below = mat;

    Matrix *temp;
    Matrix *previousTemp;
    int lines, columns;

    printf("Insira matriz: ");
    scanf("%d %d", &lines, &columns);

    temp = mat;
    for ( int i = 1; i <= lines; i++ ) {
        Matrix *newMatNode = (Matrix *) malloc (sizeof(Matrix));
        newMatNode->line = i;
        newMatNode->column = -1;
        newMatNode->right = newMatNode;

        temp->below = newMatNode;
        temp = temp->below;
    }
    temp->below = mat;

    temp = mat;
    for ( int i = 1; i <= columns; i++ ) {
        Matrix *newMatNode = (Matrix *) malloc (sizeof(Matrix));
        newMatNode->line = -1;
        newMatNode->column = i;
        newMatNode->below = newMatNode;

        temp->right = newMatNode;
        temp = temp->right;
    }
    temp->right = mat;

    int line, column;
    float value;

    do {
        scanf( "%d", &line );
        if ( line != 0 ) {
            scanf( "%d", &column );
            if ( column != 0 ) {
                scanf( "%f", &value );
            }
        }
    
        if ( line != 0 && column != 0 ) {

            Matrix* newNode = (Matrix *)malloc(sizeof(Matrix));
            newNode->right = NULL;
            newNode->below = NULL;
            newNode->line = line;
            newNode->column = column;
            newNode->info = value;

            for ( temp = mat; temp->line < line; temp = temp->below );
            previousTemp = temp;
            temp = temp->right;
            while ( temp->column < column && temp->column != -1 ) {
                previousTemp = temp;
                temp = temp->right;
            }

            previousTemp->right = newNode;
            newNode->right = temp;

            for ( temp = mat; temp->column < column; temp = temp->right );
            previousTemp = temp;
            temp = temp->below;
            while ( temp->line < line && temp->line != -1 ) {
                previousTemp = temp;
                temp = temp->below;
            }

            previousTemp->below = newNode;
            newNode->below = temp;

        } else {
            break;
        }

    } while ( 1 );

    return mat;
}

void matrix_destroy( Matrix* m ) {
    Matrix* tempLine = m->below;
    
    while ( tempLine != m ) {
        Matrix* currentLine = tempLine;
        Matrix* otherTemp = currentLine->right;
        while ( otherTemp != currentLine ) {
            Matrix* newMatNode = otherTemp;
            otherTemp = otherTemp->right;

            free ( newMatNode );
        }
        tempLine = tempLine->below;
        free ( currentLine );
    }
    free ( m );
}

void matrix_print( Matrix* m ) {

    for ( Matrix* temp = m->below; temp != m; temp = temp->below ) {
        for ( Matrix* otherTemp = temp->right; otherTemp != temp; otherTemp = otherTemp->right ) {
            printf("%.2f ", otherTemp->info);
        } 
        printf("\n");
    }
    printf("\n");
}

Matrix* matrix_add(Matrix* m, Matrix* n) {
    Matrix* tempM = m;
    Matrix* tempN = n;
    int lines, columns;

    Matrix* resultSum = (Matrix*)malloc(sizeof(Matrix));
    resultSum->line = -1;
    resultSum->column = -1;
    resultSum->right = resultSum;
    resultSum->below = resultSum;

    int i = 1;
    for (Matrix* temp = m->below; temp != m; temp = temp->below) {
        i++;
    }

    int j = 1;
    for (Matrix* temp = n->below; temp != n; temp = temp->below) {
        j++;
    }

    lines = (i > j) ? i : j;

    Matrix* temp = resultSum;
    for (int i = 0; i < lines; i++) {
        Matrix* newMatNode = ( Matrix* )malloc(sizeof(Matrix));
        newMatNode->line = i;
        newMatNode->column = -1;
        newMatNode->right = newMatNode;
        temp->below = newMatNode;
        temp = temp->below;
    }
    temp->below = resultSum;

    i = 1;
    for (Matrix* temp = m->right; temp != m; temp = temp->right) {
        i++;
    }

    j = 1;
    for (Matrix* temp = n->right; temp != n; temp = temp->right) {
        j++;
    }

    columns = (i > j) ? i : j;

    temp = resultSum;
    for (int i = 0; i < columns; i++) {
        Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
        newMatNode->line = -1;
        newMatNode->column = i;
        newMatNode->below = newMatNode;
        temp->right = newMatNode;
        temp = temp->right;
    }
    temp->right = resultSum;

    tempM = m->below;
    tempN = n->below;
    int first = 1;
    while ( tempM != m && tempN != n ) {

        if (tempM->line == tempN->line && tempM->column == tempN->column
            && tempM->column > 0 && tempN->column > 0 ) {

            Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
            newMatNode->line = tempM->line;
            newMatNode->column = tempM->column;
            newMatNode->info = tempM->info + tempN->info;
            newMatNode->right = NULL;
            newMatNode->below = NULL;

            tempM = tempM->right;
            tempN = tempN->right;

            pointMatrix( resultSum, newMatNode );

        } else if ( tempM->column < tempN->column && tempM->column > 0 ) {
            Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
            newMatNode->line = tempM->line;
            newMatNode->column = tempM->column;
            newMatNode->info = tempM->info;
            newMatNode->right = NULL;
            newMatNode->below = NULL;

            tempM = tempM->right;

            pointMatrix( resultSum, newMatNode );

        } else if ( tempN->column < tempM->column && tempN->column > 0 ){
            Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
            newMatNode->line = tempN->line;
            newMatNode->column = tempN->column;
            newMatNode->info = tempN->info;
            newMatNode->right = NULL;
            newMatNode->below = NULL;

            tempN = tempN->right;

            pointMatrix( resultSum, newMatNode );
        } else {
            if ( tempM->column == -1 && first ) tempM = tempM->right;
            if ( tempN->column == -1 && first ) tempN = tempN->right;
        }

        first = 0;

        if ( tempM->column == -1 && tempN->column == -1) {
            tempM = tempM->below;
            tempN = tempN->below;
            first = 1;
        }
    }
    return resultSum;
}

void pointMatrix ( Matrix* result, Matrix* newMatNode ) {
    Matrix *temp;
    Matrix* previousTemp;

    for (temp = result; temp->line < newMatNode->line; temp = temp->below);
    do {
        previousTemp = temp;
        temp = temp->right;
    } while ( temp->column < newMatNode->column && temp->column != -1 );
    newMatNode->right = temp->right;
    previousTemp->right = newMatNode;

    for (temp = result; temp->column < newMatNode->column; temp = temp->right) {
        previousTemp = temp;
    }
    do {
        previousTemp = temp;
        temp = temp->below;
    } while ( temp->line < newMatNode->line && temp->line != -1 );
    newMatNode->below = temp->below;
    previousTemp->below = newMatNode;
}

Matrix* matrix_multiply( Matrix* m, Matrix* n ) {
     Matrix* tempM = m;
    Matrix* tempN = n;
    int lines, columns;

    Matrix* resultMultiply = (Matrix*)malloc(sizeof(Matrix));
    resultMultiply->line = -1;
    resultMultiply->column = -1;
    resultMultiply->right = resultMultiply;
    resultMultiply->below = resultMultiply;

    int i = 1;
    for (Matrix* temp = m->below; temp != m; temp = temp->below) {
        i++;
    }

    int j = 1;
    for (Matrix* temp = n->below; temp != n; temp = temp->below) {
        j++;
    }

    lines = (i > j) ? i : j;

    Matrix* temp = resultMultiply;
    for (int i = 0; i < lines; i++) {
        Matrix* newMatNode = ( Matrix* )malloc(sizeof(Matrix));
        newMatNode->line = i;
        newMatNode->column = -1;
        newMatNode->right = newMatNode;
        temp->below = newMatNode;
        temp = temp->below;
    }
    temp->below = resultMultiply;

    i = 1;
    for (Matrix* temp = m->right; temp != m; temp = temp->right) {
        i++;
    }

    j = 1;
    for (Matrix* temp = n->right; temp != n; temp = temp->right) {
        j++;
    }

    columns = (i > j) ? i : j;

    temp = resultMultiply;
    for (int i = 0; i < columns; i++) {
        Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
        newMatNode->line = -1;
        newMatNode->column = i;
        newMatNode->below = newMatNode;
        temp->right = newMatNode;
        temp = temp->right;
    }
    temp->right = resultMultiply;

    tempM = m->below;
    tempN = n->below;
    int first = 1;
    while ( tempM != m && tempN != n ) {

        if (tempM->line == tempN->line && tempM->column == tempN->column
            && tempM->column > 0 && tempN->column > 0 ) {

            Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
            newMatNode->line = tempM->line;
            newMatNode->column = tempM->column;
            newMatNode->info = tempM->info * tempN->info;
            newMatNode->right = NULL;
            newMatNode->below = NULL;

            tempM = tempM->right;
            tempN = tempN->right;

            pointMatrix( resultMultiply, newMatNode );

        } else if ( tempM->column < tempN->column && tempM->column > 0 ) {
            Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
            newMatNode->line = tempM->line;
            newMatNode->column = tempM->column;
            newMatNode->info = tempM->info;
            newMatNode->right = NULL;
            newMatNode->below = NULL;

            tempM = tempM->right;

            pointMatrix( resultMultiply, newMatNode );

        } else if ( tempN->column < tempM->column && tempN->column > 0 ){
            Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
            newMatNode->line = tempN->line;
            newMatNode->column = tempN->column;
            newMatNode->info = tempN->info;
            newMatNode->right = NULL;
            newMatNode->below = NULL;

            tempN = tempN->right;

            pointMatrix( resultMultiply, newMatNode );
        } else {
            if ( tempM->column == -1 && first ) tempM = tempM->right;
            if ( tempN->column == -1 && first ) tempN = tempN->right;
        }

        first = 0;

        if ( tempM->column == -1 && tempN->column == -1) {
            tempM = tempM->below;
            tempN = tempN->below;
            first = 1;
        }
    }
    return resultMultiply;
}

Matrix* matrix_transpose( Matrix* m ) {
    Matrix *mT = matrix_create();
    int line = m->line;
    int column = m->column;

    for ( int i = 1; i <= column; i++ ) {
        for ( int j = 1; j <= line; j++ ) {
            Matrix* temp = m;
            while (temp->below != m && temp->line < j) {
                temp = temp->below;
            }
            while (temp->right != m && temp->column < i) {
                temp = temp->right;
            }
            if (temp->line == j && temp->column == i) {
                Matrix *newMatNode = (Matrix *)malloc(sizeof(Matrix));
                newMatNode->line = i;
                newMatNode->column = j;
                newMatNode->info = temp->info;
                newMatNode->right = mT->right;
                newMatNode->below = mT->below;

                mT->below->right = newMatNode;
                mT->below = newMatNode;
            }
        }
    }
    return mT;
}

float matrix_getelem( Matrix* m, int x, int y ) {
    Matrix* temp = m;

    for ( temp = m; temp->line < x; temp = temp->below ) {
        if ( temp->line == x ) {
            for ( temp = m; temp->column < y; temp = temp->right ) {
                if ( temp->column == y ) {
                    return temp->info;
                }
            }
        }
    }
    return -1;
}

void matrix_setelem(Matrix* m, int x, int y, float elem) {
    Matrix* temp = m->below;
    
    for (temp = m; temp->line < x; temp = temp->below) {
        if (temp->line != x) {
            Matrix* newTemp = (Matrix*)malloc(sizeof(Matrix));
            newTemp->line = x;
            newTemp->column = m->column;
            newTemp->right = newTemp;
            newTemp->below = temp;
            m->below = newTemp;
            temp = newTemp;
        }
    }
    
    Matrix* otherTemp = temp;
    for (otherTemp = m; otherTemp->column < y; otherTemp = otherTemp->right) {
        for (; otherTemp->right != temp && otherTemp->right->column <= y; otherTemp = otherTemp->right);
    }
    
    if (otherTemp->right == temp || otherTemp->right->column != y) {
        Matrix* newMatNode = (Matrix*)malloc(sizeof(Matrix));
        newMatNode->line = x;
        newMatNode->column = y;
        newMatNode->info = elem;
        newMatNode->right = otherTemp->right;
        otherTemp->right = newMatNode;

        Matrix* column = m;
        for (; column->right != m && column->right->column < y; column = column->right);

        if (column->right == m || column->right->column != y) {
            newMatNode->below = column->right;
            column->right = newMatNode;
        }
    } else {
        otherTemp->right->info = elem;
    }
}