import matplotlib.pyplot as plt
import scipy.stats

# mean, deviation, data
data = {
"lgc64": (589859.533333, 634.303488525, [589347, 589706, 590512, 589554, 589956, 589123, 589736, 590338, 589844, 591292, 589500, 589081, 589291, 589627, 59098]),
"lgc64shift": (590205.933333, 653.639601683, [589800, 589190, 589249, 590556, 590022, 590485, 590653, 589640, 590378, 590994, 590222, 590716, 589232, 590478, 591474]),
"mrg2":( 590299.533333, 534.51883243, [589809, 590071, 590872, 590688, 589771, 590895, 589628, 590700, 590360, 589913, 589745, 591044, 590428, 591071, 589498]),
"mrg3":( 590299.2, 449.258233091, [590583, 590294, 590229, 590315, 590381, 591263, 589279, 590814, 589928, 590290, 589835, 589859, 590284, 590636, 590498]),
"yarn2":( 590113.6, 687.05388435, [589374, 590178, 589448, 591321, 589544, 590697, 589827, 591033, 590741, 589349, 590588, 589911, 590895, 589682, 589116]),
"yarn3":( 590015.6, 474.807020448, [590692, 590150, 590279, 590497, 588942, 589786, 589996, 590592, 590114, 589647, 590088, 590300, 589103, 590058, 589990])	
}

# for engine in data:
    # print(engine, end='\t\t\t')
    # mean = data[engine][0]
    # deviation = data[engine][1]
    # print("\([" +  str(round(mean - 2*deviation)) + "," + str(round(mean+ 2*deviation)) + "]\)")


# mrg2 = data["mrg2"]
for engine1 in data:
    for engine2 in data:
        if engine1 != "lgc64" and engine2 != "lgc64" and engine1 != engine2:
            print(engine1, engine2, round(scipy.stats.ttest_rel(data[engine1][2], data[engine2][2])[1], 4))
            # print("z.test(" + engine1, ", " + engine2 + ", sigma.x = " + str(data[engine1][1]) + ", sigma.y = " + str(data[engine2][1]) + ")")
    # print(engine1, engine2, scipy.stats.ks_2samp(data[engine1][2], data[engine2][2]))
    # print(data[engine1][2])
    # print(engine1, scipy.stats.kstest(data[engine1][2], "norm"))
    # print(engine1, scipy.stats.shapiro(data[engine1][2]))





    # plt.figure()
    # plt.title("Boxplot")
    # plt.xlabel('Amount of people infected at the end')
    # plt.ylabel('Amount of occurences')
    # plt.boxplot(data[engine][2], 0, 'rs', 0, 0.75)
    # plt.savefig("boxplot_" + engine + ".png")
