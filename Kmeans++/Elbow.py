import matplotlib
matplotlib.use('Agg')
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
from sklearn import datasets
import numpy as np
import matplotlib.patches as patch

#load data
iris = datasets.load_iris()
data = iris.data

#run kmeans algorithm and build the inertia list
inertia = []
for k in range(1,11):
    kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0).fit(data)
    inertia.append(kmeans.inertia_)

#x axis array - all the Ks
x = np.arange(1,11,1)

#get handles for the plot
fig, ax = plt.subplots()

#plot the inertia of each number of cluster (K)
ax.plot(x, inertia)

#change the design and tags of the plot
ax.set_xticks(x)
ax.set_xlabel('K')
ax.set_ylabel('Average Dispersion')
ax.set_title('Elbow Method for selection of optimal "K" clusters')

#add elbow point 1
ax.add_patch(patch.Ellipse((2, inertia[1]), width=0.5, height=50, facecolor='none', linewidth=1, linestyle='--', edgecolor='black'))
ax.annotate('Elbow Point 1', xy=(2.3, inertia[1] + 10), xytext=(3.5, inertia[1] + 50), arrowprops=dict(facecolor='black', shrink = 0.05))

#add elbow point 2
ax.add_patch(patch.Ellipse((3, inertia[2]), width=0.5, height=50, facecolor='none', linewidth=1, linestyle='--', edgecolor='black'))
ax.annotate('Elbow Point 2', xy=(3.3, inertia[2] + 10), xytext=(4.5, inertia[2] + 50), arrowprops=dict(facecolor='black', shrink = 0.05))

#save and close the figure (plot)
fig.savefig('elbow.png')
plt.close(fig)
    
