#include <tb_menu.h>

int tb_menu_draw(struct TbMenu* menu) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  return TBM_SUCCESS;
}

int tb_menu_init(struct TbMenu* menu) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  menu->selected = false;
  menu->selection = 0;

  menu->items = NULL;
  menu->items_length = 0;

  menu->filtered_items = NULL;
  menu->filtered_items_length = 0;
  return TBM_SUCCESS;
}

int tb_menu_uninit(struct TbMenu* menu) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  if(menu->filtered_items != NULL)
    free(menu->filtered_items);

  menu->filtered_items = NULL;
  menu->filtered_items_length = 0;

  return TBM_SUCCESS;
}
