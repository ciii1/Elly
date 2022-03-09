clear
gcc src/main.c src/dstr.c src/lexer.c src/prettyerr.c src/compiler.c src/generator.c src/ht/ht.c src/darr.c src/backend/c_backend.c -o elly.out -fmax-errors=1
