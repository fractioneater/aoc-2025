# Not my code! https://www.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/

from functools import cache
from itertools import combinations, product

def patterns(coefficients: list[tuple[int, ...]]) -> dict[tuple[int, ...], dict[tuple[int, ...], int]]:
  num_buttons = len(coefficients)
  num_variables = len(coefficients[0])
  out = {parity_pattern: {} for parity_pattern in product(range(2), repeat=num_variables)}
  for num_pressed_buttons in range(num_buttons + 1):
    for buttons in combinations(range(num_buttons), num_pressed_buttons):
      pattern = tuple(map(sum, zip((0,) * num_variables, *(coefficients[i] for i in buttons))))
      parity_pattern = tuple(i % 2 for i in pattern)
      if pattern not in out[parity_pattern]:
        out[parity_pattern][pattern] = num_pressed_buttons
  return out

def solve_single(coefficients: list[tuple[int, ...]], outer_goal: tuple[int, ...]) -> int:
  pattern_costs = patterns(coefficients)

  @cache
  def solve_single_aux(goal: tuple[int, ...]) -> int:
    if all(i == 0 for i in goal): return 0
    answer = 1000000
    for pattern, pattern_cost in pattern_costs[tuple(i % 2 for i in goal)].items():
      if all(i <= j for i, j in zip(pattern, goal)):
        new_goal = tuple((j - i) // 2 for i, j in zip(pattern, goal))
        answer = min(answer, pattern_cost + 2 * solve_single_aux(new_goal))
    return answer

  return solve_single_aux(outer_goal)

def solve(raw: str):
  score = 0
  lines = raw.splitlines()
  for I, L in enumerate(lines, 1):
    _, *coefficients, goal = L.split()
    goal = tuple(int(i) for i in goal[1:-1].split(","))
    coefficients = [[int(i) for i in r[1:-1].split(",")] for r in coefficients]
    coefficients = [tuple(int(i in r) for i in range(len(goal))) for r in coefficients]

    subscore = solve_single(coefficients, goal)
    print(f'Line {I}/{len(lines)}: answer {subscore}')
    score += subscore
  print(score)

solve(open('input/day10.txt').read())
