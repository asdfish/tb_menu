#include <tb_menu.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

int main(void) {
  struct TbMenuItem items[] = {
    { TB_WHITE, TB_BLACK, "asdf" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "fdsa" },
    { TB_WHITE, TB_BLACK, "asdf" },
    { TB_WHITE, TB_BLACK, "asdf" },
    { TB_WHITE, TB_BLACK, "asdf" },
    { TB_WHITE, TB_BLACK, "asdf" },
  };

  struct TbMenu menu;
  menu.x = 0;
  menu.y = 0;
  menu.width = 10;
  menu.height = 10;
  menu.background = TB_BLACK;
  menu.background_reversed = TB_WHITE;

  tb_menu_init(&menu);

  tb_menu_set_items(&menu, (struct TbMenuItem*) items, ARRAY_LENGTH(items));
  tb_menu_filter(&menu, "as");

  tb_init();

  while(true) {
    tb_menu_draw(&menu);
    tb_present();

    struct tb_event ev;
    tb_poll_event(&ev);

    switch(ev.ch) {
      case 'q':
        goto exit_loop;
      case 'j':
        tb_menu_move_cursor(&menu, 1);
        break;
      case 'k':
        tb_menu_move_cursor(&menu, -1);
        break;
    }
  }
exit_loop:

  tb_shutdown();
  tb_menu_uninit(&menu);
  return 0;
}
