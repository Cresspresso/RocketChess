
#include "main_menu_button_entity.hpp"



MainMenuButtonEntity::MainMenuButtonEntity()
{
	buttonEntity.textEntity.material.tint = vec3(0);
}



ReturnCode MainMenuButtonEntity::render()
{
	return buttonEntity.render();
}
