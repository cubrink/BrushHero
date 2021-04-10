# %%
import pandas as pd
import matplotlib.pyplot as plt

# %%
def run_analysis(filename):
    df = pd.read_csv(filename)
    fig, axes = plt.subplots(nrows=2, ncols=2)
    df_['a_mag'] = (df_['a_x']**2 + df_['a_y']**2 + df_['a_y']**2).apply(sqrt)
    df_['j_mag'] = (df_['j_x']**2 + df_['j_y']**2 + df_['j_y']**2).apply(sqrt)

    df_[['a_x', 'j_x']].plot(title='Acceleration/Jerk along x-axis', ax=axes[0,0], figsize=(20,20))
    df_[['a_y', 'j_y']].plot(title='Acceleration/Jerk along y-axis', ax=axes[0,1], figsize=(20,20))
    df_[['a_z', 'j_z']].plot(title='Acceleration/Jerk along z-axis', ax=axes[1,0], figsize=(20,20))
    df_[['a_mag', 'j_mag']].plot(title='Acceleration/Jerk magnitude', ax=axes[1,1], figsize=(20,20))

# %%
run_analysis('BackAndForthMovement.csv')

# %%
