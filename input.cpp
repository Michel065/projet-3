#include "input.h"

AppInput::AppInput()
{

}

void AppInput::keyPressed(int key)
{
	// Logique de gestion des appuis clavier
	char message[32];
	snprintf(message, sizeof(message), "touche : %c", key);
	qDebug(message);
}
