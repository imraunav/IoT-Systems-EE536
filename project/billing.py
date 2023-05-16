import pandas as pd
import numpy as np

houses = ["house-1", "house-2", "house-3", "house-4", "house-5"]

for house in houses:
	df = pd.read_csv(house+".csv", header=None)
	readings = df.iloc[-24:, 0].to_numpy(dtype="int") # select first column only
	peakPeriod = df.iloc[-24:, 1].to_numpy(dtype="int") # select second columns only
	
	power_sum = 0
	for r, p in zip(readings, peakPeriod):
		if p == 0: # non-peak
			power_sum += r*1;
		else: # peak
			power_sum += r*2;
	print(f"{house} bill for the day: {power_sum}")


