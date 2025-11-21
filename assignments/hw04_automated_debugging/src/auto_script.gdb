set pagination off
set logging file address_layout_32_simple_output.txt
set logging overwrite on
set logging enabled on
break main
run
info proc mappings
printf "\n--- VARIABLE ADDRESSES ---\n"
p &local_var_1
p &local_var_2
p ptr_1
p ptr_2
p &static_var_1
p &static_var_2
p &global_var_1
p &global_var_2
p &global_uninit_var_1
p &global_uninit_var_2
printf "\n--- DISASSEMBLE main ---\n"
disassemble main
set $p1 = ptr_1
set $p2 = ptr_2
printf "\n--- DUMP AT ptr_1 (8 words) ---\n"
x/8x $p1
printf "\n--- DUMP AT ptr_2 (8 words) ---\n"
x/8x $p2
printf "\n--- SYMBOLS / VARIABLES ---\n"
info variables
set logging off
quit

