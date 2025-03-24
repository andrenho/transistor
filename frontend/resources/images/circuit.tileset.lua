return {
   tile_size = 16,
   
   tiles = {
      
      tile            = { 2, 2 },
      
      shadow_rect     = { 5, 4 },
      shadow_square   = { 6, 4 },
      shadow_circle   = { 7, 4 },
      button_off      = { 6, 2 },
      button_on       = { 6, 3 },
      led_off         = { 7, 2 },
      led_on          = { 7, 3 },
      vcc             = { 8, 2 },
      npn             = { 5, 2 },
      pnp             = { 5, 3 },
      
      tb_arrow        = { 0,  8 },
      tb_vcc          = { 0,  9 },
      tb_button       = { 0, 10 },
      tb_led          = { 0, 11 },
      tb_npn          = { 0, 12 },
      tb_pnp          = { 0, 13 },
      tb_logic_gates  = { 1,  8 },
      tb_digital      = { 1, 11 },
      tb_memory       = { 1, 10 },
      tb_cpu          = { 1,  9 },
      
      board_top_left     = { 0, 0, 2, 2 },
      board_top          = { 2, 0, 1, 2 },
      board_top_right    = { 3, 0, 2, 2 },
      board_left         = { 0, 2, 2, 1 },
      board_right        = { 3, 2, 2, 1 },
      board_bottom_left  = { 0, 3, 2, 2 },
      board_bottom       = { 2, 3, 1, 2 },
      board_bottom_right = { 3, 3, 2, 2 },

      wire_top_on_n_1   = { 0, 7 },
      wire_top_on_e_1   = { 1, 7 },
      wire_top_on_w_1   = { 2, 7 },
      wire_top_on_s_1   = { 3, 7 },
      wire_top_off_n_1  = { 0, 5 },
      wire_top_off_e_1  = { 1, 5 },
      wire_top_off_w_1  = { 2, 5 },
      wire_top_off_s_1  = { 3, 5 },

      ic_dot_n = { 10, 3 },
      ic_dot_w = { 11, 3 },
      ic_dot_e = { 10, 4 },
      ic_dot_s = { 11, 4 },

      ic_pin_n = { 11, 5 },
      ic_pin_e = { 10, 5 },
      ic_pin_w = { 11, 6 },
      ic_pin_s = { 10, 6 },

      logic_gates_nw = { 12, 3 },
      logic_gates_n = { 13, 3 },
      logic_gates_ne = { 14, 3 },
      logic_gates_w = { 12, 4 },
      logic_gates_center = { 13, 4 },
      logic_gates_e = { 14, 4 },
      logic_gates_sw = { 12, 5 },
      logic_gates_s = { 13, 5 },
      logic_gates_se = { 14, 5 },

      digital_nw = { 15, 3 },
      digital_n = { 16, 3 },
      digital_ne = { 17, 3 },
      digital_w = { 15, 4 },
      digital_center = { 16, 4 },
      digital_e = { 17, 4 },
      digital_sw = { 15, 5 },
      digital_s = { 16, 5 },
      digital_se = { 17, 5 },

      memory_nw = { 12, 6 },
      memory_n = { 13, 6 },
      memory_ne = { 14, 6 },
      memory_w = { 12, 7 },
      memory_center = { 13, 7 },
      memory_e = { 14, 7 },
      memory_sw = { 12, 8 },
      memory_s = { 13, 8 },
      memory_se = { 14, 8 },

      cpu_nw = { 15, 6 },
      cpu_n = { 16, 6 },
      cpu_ne = { 17, 6 },
      cpu_w = { 15, 7 },
      cpu_center = { 16, 7 },
      cpu_e = { 17, 7 },
      cpu_sw = { 15, 8 },
      cpu_s = { 16, 8 },
      cpu_se = { 17, 8 },
   }
}
