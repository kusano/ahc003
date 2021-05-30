import optuna
import os
import pwn
import sys

pwn.context.log_level = "error"

prog = sys.argv[1]

def objective(trial):
  #hv_min = trial.suggest_float("hv_min", 0, 5000)
  #hv_min = trial.suggest_float("hv_min", 0, 2000)
  hv_min = trial.suggest_float("hv_min", 1000, 2000)
  #hv_max = trial.suggest_float("hv_max", 5000, 10000)
  hv_max = trial.suggest_float("hv_max", 7000, 10000)
  #hv_init = trial.suggest_float("hv_init", hv_min, hv_max)
  hv_init = trial.suggest_float("hv_init", 2000, 3000)
  #hv_learn_rate = trial.suggest_float("hv_learn_rate", 0, 1)
  #hv_learn_rate = trial.suggest_float("hv_learn_rate", 0.1, 0.6)
  hv_learn_rate = trial.suggest_float("hv_learn_rate", 0.3, 0.6)
  #delta_min = trial.suggest_float("delta_min", -5000, 0)
  #delta_min = trial.suggest_float("delta_min", -3000, 0)
  delta_min = trial.suggest_float("delta_min", -3000, -1500)
  #delta_max = trial.suggest_float("delta_max", 0, 5000)
  delta_max = trial.suggest_float("delta_max", 1000, 4000)
  #delta_init = trial.suggest_float("delta_init", delta_min, delta_max)
  delta_init = trial.suggest_float("delta_init", delta_min, 0)
  #delta_learn_rate = trial.suggest_float("delta_learn_rate", 0, 1)
  #delta_learn_rate = trial.suggest_float("delta_learn_rate", 0, 0.2)
  delta_learn_rate = trial.suggest_float("delta_learn_rate", 0.05, 0.2)

  os.system(
    f"g++ -O2 " +
    f"-DPARAM_HV_MIN={hv_min} "+
    f"-DPARAM_HV_MAX={hv_max} "+
    f"-DPARAM_HV_INIT={hv_init} "+
    f"-DPARAM_HV_LEARN_RATE={hv_learn_rate} "+
    f"-DPARAM_DELTA_MIN={delta_min} "+
    f"-DPARAM_DELTA_MAX={delta_max} "+
    f"-DPARAM_DELTA_INIT={delta_init} "+
    f"-DPARAM_DELTA_LEARN_RATE={delta_learn_rate} "+
    f"-o {prog} A.cpp")

  s = pwn.process(f"./test.sh {prog}", shell=True)
  s.readuntil("score: ")
  score = int(s.recvline().decode()[:-1])
  s.close()
  return score

study = optuna.create_study(
  study_name="ahc003_9",
  storage="sqlite:///db.sqlite3",
  load_if_exists=True,
  direction="maximize")
study.optimize(objective, n_trials=1000)
print(study.best_params)
print(study.best_value)
