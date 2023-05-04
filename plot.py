import csv
import matplotlib.pyplot as plt

# Read the CSV file into a list of dictionaries
with open('ghc-5000.csv') as f:
    reader = csv.DictReader(f)
    data = [row for row in reader]

# Compute the average score and time for rows with the same METHOD, AGENTNUM, ITERNUM, and SOURCE
averages = {}
counts = {}
for row in data:
    key = (row['METHOD'], int(row['AGENTNUM']), int(row['ITERNUM']), row['SOURCE'])
    if key in averages:
        averages[key]['SCORE'] += float(row['SCORE'])
        averages[key]['TIME'] += float(row['TIME'])
        counts[key] += 1
    else:
        averages[key] = {'SCORE': float(row['SCORE']), 'TIME': float(row['TIME'])}
        counts[key] = 1
for key in averages:
    averages[key]['SCORE'] /= counts[key]
    averages[key]['TIME'] /= counts[key]

# Filter the dictionary to only include rows with ITERNUM=10 and SOURCE=cleavland.csv
filtered_data = []
for key in averages:
    if key[1] == 5 and key[3] == 'cleavland.csv':
        filtered_data.append({'METHOD': key[0], 'ITERNUM': key[2], 'SCORE': averages[key]['SCORE'], 'TIME': averages[key]['TIME']})

# Plot the data points
fig, ax = plt.subplots()
for method in set(row['METHOD'] for row in filtered_data):
    method_data = [row for row in filtered_data if row['METHOD'] == method]
    agentnums = [row['ITERNUM'] for row in method_data]
    times = [row['TIME'] for row in method_data]
    ax.plot(agentnums, times, label=method)
ax.legend()
ax.set_xlabel('ITERNUM')
ax.set_ylabel('TIME')
# Save the plot to a file
fig.savefig('iter.png')

plt.show()
