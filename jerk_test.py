# %%
import pandas as pd
import matplotlib.pyplot as plt

# %%
def run_analysis(filename):
    df_ = pd.read_csv(filename)
    fig, axes = plt.subplots(nrows=2, ncols=2)
    df_['a_mag'] = (df_['a_x']**2 + df_['a_y']**2 + df_['a_y']**2).apply(sqrt)
    df_['j_mag'] = (df_['j_x']**2 + df_['j_y']**2 + df_['j_y']**2).apply(sqrt)

    df_[['a_x', 'j_x']].plot(title='Acceleration/Jerk along x-axis', ax=axes[0,0], figsize=(20,20))
    df_[['a_y', 'j_y']].plot(title='Acceleration/Jerk along y-axis', ax=axes[0,1], figsize=(20,20))
    df_[['a_z', 'j_z']].plot(title='Acceleration/Jerk along z-axis', ax=axes[1,0], figsize=(20,20))
    df_[['a_mag', 'j_mag']].plot(title='Acceleration/Jerk magnitude', ax=axes[1,1], figsize=(20,20))
    return df_

# %%
back_forth = run_analysis('BackAndForthMovement.csv')

# %%
up_down = run_analysis('UpAndDownMovement.csv')
# %%
def run_comparison(file_a, file_b, jerk_only=True, transform=lambda x: x):
    df_a = pd.read_csv(file_a).apply(transform)
    df_b = pd.read_csv(file_b).apply(transform)

    fig, axes = plt.subplots(nrows=4, ncols=2)
    df_a['a_mag'] = (df_a['a_x']**2 + df_a['a_y']**2 + df_a['a_y']**2).apply(sqrt)
    df_b['a_mag'] = (df_b['a_x']**2 + df_b['a_y']**2 + df_b['a_y']**2).apply(sqrt)
    df_a['j_mag'] = (df_a['j_x']**2 + df_a['j_y']**2 + df_a['j_y']**2).apply(sqrt)
    df_b['j_mag'] = (df_b['j_x']**2 + df_b['j_y']**2 + df_b['j_y']**2).apply(sqrt)

    if jerk_only:
        df_a[['j_x']].plot(title=f'{file_a} along x-axis', ax=axes[0,0], figsize=(20,20))
        df_b[['j_x']].plot(title=f'{file_b} along x-axis', ax=axes[0,1], figsize=(20,20))
        df_a[['j_y']].plot(title=f'{file_a} along y-axis', ax=axes[1,0], figsize=(20,20))
        df_b[['j_y']].plot(title=f'{file_b} along y-axis', ax=axes[1,1], figsize=(20,20))
        df_a[['j_z']].plot(title=f'{file_a} along z-axis', ax=axes[2,0], figsize=(20,20))
        df_b[['j_z']].plot(title=f'{file_b} along z-axis', ax=axes[2,1], figsize=(20,20))
        df_a[['j_mag']].plot(title=f'{file_a} magnitude', ax=axes[3,0], figsize=(20,20))
        df_b[['j_mag']].plot(title=f'{file_b} magnitude', ax=axes[3,1], figsize=(20,20))
    else:
        df_a[['a_x', 'j_x']].plot(title=f'{file_a} along x-axis', ax=axes[0,0], figsize=(20,20))
        df_b[['a_x', 'j_x']].plot(title=f'{file_b} along x-axis', ax=axes[0,1], figsize=(20,20))
        df_a[['a_y', 'j_y']].plot(title=f'{file_a} along y-axis', ax=axes[1,0], figsize=(20,20))
        df_b[['a_y', 'j_y']].plot(title=f'{file_b} along y-axis', ax=axes[1,1], figsize=(20,20))
        df_a[['a_z', 'j_z']].plot(title=f'{file_a} along z-axis', ax=axes[2,0], figsize=(20,20))
        df_b[['a_z', 'j_z']].plot(title=f'{file_b} along z-axis', ax=axes[2,1], figsize=(20,20))
        df_a[['a_mag', 'j_mag']].plot(title=f'{file_a} magnitude', ax=axes[3,0], figsize=(20,20))
        df_b[['a_mag', 'j_mag']].plot(title=f'{file_b} magnitude', ax=axes[3,1], figsize=(20,20))

    return df_a, df_b
# %%
run_comparison('BackAndForthMovement.csv', 'UpAndDownMovement.csv', transform=lambda x: x**2)
# %%
back_forth.apply(lambda x: x**2)
# %%
run_comparison('BackAndForthMovement.csv', 'UpAndDownMovement.csv', transform=lambda x: x*abs(x))


# %%
