import optuna
import os
import pwn
import sys

pwn.context.log_level = "error"

prog = sys.argv[1]

def objective(trial):
  delta_min = trial.suggest_float("delta_min", -10000, 0)
  delta_max = trial.suggest_float("delta_max", 0, 10000)
  delta_init = trial.suggest_float("delta_init", delta_min, delta_max)

  os.system(
    f"g++ -O2 " +
    f"-DPARAM_DELTA_MIN={delta_min} "+
    f"-DPARAM_DELTA_MAX={delta_max} "+
    f"-DPARAM_DELTA_INIT={delta_init} "+
    f"-o {prog} A.cpp")

  s = pwn.process(f"./test.sh {prog}", shell=True)
  s.readuntil("score: ")
  score = int(s.recvline().decode()[:-1])
  s.close()
  return score

study = optuna.create_study(
  study_name="ahc003_5",
  storage="sqlite:///db.sqlite3",
  load_if_exists=True,
  direction="maximize")
study.optimize(objective, n_trials=1000)
print(study.best_params)
print(study.best_value)
