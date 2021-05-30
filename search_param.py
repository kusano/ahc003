import optuna
import os
import pwn
import sys

pwn.context.log_level = "error"

def objective(trial):
  hv_min = trial.suggest_float("hv_min", 0, 5000)
  hv_max = trial.suggest_float("hv_max", 5000, 10000)
  hv_init = trial.suggest_float("hv_init", hv_min, hv_max)
  delta_min = trial.suggest_float("delta_min", -10000, 0)
  delta_max = trial.suggest_float("delta_max", 0, 10000)
  delta_init = trial.suggest_float("delta_init", delta_min, delta_max)
  delta_rate = trial.suggest_float("delta_rate", 0, 1)

  os.system(
    f"g++ -O2 " +
    f"-DPARAM_HV_MIN={hv_min} "+
    f"-DPARAM_HV_MAX={hv_max} "+
    f"-DPARAM_HV_INIT={hv_init} "+
    f"-DPARAM_DELTA_MIN={delta_min} "+
    f"-DPARAM_DELTA_MAX={delta_max} "+
    f"-DPARAM_DELTA_INIT={delta_init} "+
    f"-DPARAM_DELTA_RATE={delta_rate} "+
    f"-o A A.cpp")

  s = pwn.process("./test.sh")
  s.readuntil("score: ")
  score = int(s.recvline().decode()[:-1])
  s.close()
  return score

study = optuna.create_study(
  study_name="ahc003_3",
  storage="sqlite:///db.sqlite3",
  load_if_exists=True,
  direction="maximize")
study.optimize(objective, n_trials=1000)
print(study.best_params)
print(study.best_value)
