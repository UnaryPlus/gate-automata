#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char oct;

// evolve pattern for one generation
// the returned array always has width 2 greater than the input array
oct* step(oct* rule_table, oct* old_gen, int w) {
  oct* new_gen = malloc(w + 2);
  new_gen[0] = rule_table[old_gen[0] >> 2];
  if(w == 1) {
    new_gen[1] = rule_table[old_gen[0] & 2];
  }
  else {
    new_gen[1] = rule_table[old_gen[0] & 2 | old_gen[1] >> 2];
    new_gen[w] = rule_table[old_gen[w - 2] << 2 & 7 | old_gen[w - 1] & 2]; 
  }
  new_gen[w + 1] = rule_table[old_gen[w - 1] << 2 & 7]; 
  for(int i = 2; i <= w - 1; ++i) {
    // new_gen[i] depends on old_gen[i - 2, i - 1, i]
    new_gen[i] = rule_table[old_gen[i - 2] << 2 & 7 | old_gen[i - 1] & 2 | old_gen[i] >> 2];
  }
  return new_gen;
}

void draw_line(FILE* svg, int x1, int y1, int x2, int y2) {
  fprintf(svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" stroke-width=\"0.1\" />\n", x1, y1, x2, y2);
}

void draw_oct(FILE* svg, int x, int y, oct b) {
  if(b >> 2) {
    draw_line(svg, x, y, x - 1, y + 1);
  }
  if(b >> 1 & 1) {
    draw_line(svg, x, y, x, y + 1);
  }
  if(b & 1) {
    draw_line(svg, x, y, x + 1, y + 1);
  }
}

void draw_gen(FILE* svg, int start_x, int start_y, oct* gen, int w) {
  for(int i = 0; i < w; ++i) {
    draw_oct(svg, start_x + i, start_y, gen[i]);
  }
}

void run_and_draw(FILE* svg, oct* rule_table, int gens, int start_x, int start_y, oct* gen, int w) {
  draw_gen(svg, start_x, start_y, gen, w);
  for(int i = 0; i < gens; ++i) {
    oct* new_gen = step(rule_table, gen, w); 
    free(gen);
    --start_x;
    ++start_y;
    gen = new_gen;
    w += 2;
    draw_gen(svg, start_x, start_y, gen, w);
  }
}

void help(char* progname) {
  printf("Usage: %s <rule> <init> <gens> <svg_path>\n", progname);
  printf("<rule> is a sequence of 7 octal digits, specifying the rule to run.\n");
  printf("<init> is a sequence of 1 or more octal digits, specifying the initial pattern.\n");
  printf("<gens> is a positive integer: the number of generations to run.\n");
  printf("<svg_path> is the path of the output svg file.\n");
  printf("For example, try '%s 3506000 5 100 pyramid.svg'.\n", progname);
  exit(0);
}

oct check_octal(char d, char* progname) {
  if(d < '0' || d > '7') {
    printf("Error: '%c' is not an octal digit.\n", d);
    exit(1);
  }
  return d - '0';
}

int main(int argc, char** argv) {
  if(argc != 5) {
    help(argv[0]);
  }
  int rule_len = strlen(argv[1]);
  if(rule_len != 7) {
    printf("Error: invalid rulestring length (is %d, should be 7).\n", rule_len);
    exit(1);
  }
  // create rule table
  oct rule_table[8] = { 0 };
  for(int i = 0; i < 7; ++i) {
    rule_table[i + 1] = check_octal(argv[1][i], argv[0]);
  }

  // create initial pattern
  int init_w = strlen(argv[2]);
  oct* init_gen = malloc(init_w);
  for(int i = 0; i < init_w; ++i) {
    init_gen[i] = check_octal(argv[2][i], argv[0]);
  }
  int gens = atoi(argv[3]);
  if(gens <= 0) {
    printf("Error: generation count must be a positive integer (is '%s').\n", argv[3]);
    exit(1);
  }
  FILE* svg = fopen(argv[4], "w");
  if(svg == NULL) {
    printf("Error: could not open file '%s'.\n", argv[4]);
    exit(1);
  }
  fprintf(svg, "<svg viewBox=\"0 0 %d %d\" xmlns=\"http://www.w3.org/2000/svg\">\n", init_w + (gens * 2) + 3, gens + 2);
  run_and_draw(svg, rule_table, gens, gens + 2, 1, init_gen, init_w);
  fprintf(svg, "</svg>\n");
  printf("Wrote output to %s.\n", argv[4]);
  return 0;
}