import sys
import math
import unittest
from ortools.linear_solver import pywraplp
from Image import Image



def is_perfect_square(s):
    r = math.sqrt(s)
    return r == math.floor(r)




def manhattan_dist(v1, v2):

    if len(v1) != len(v2):
        raise Exception("dist.manhattan_dist: Unequal vector sizes!")

    ret = 0
    for i in range(len(v1)):
        ret += abs(v1[i] - v2[i])
    
    return ret




def euclidean_dist(v1,v2):

    if len(v1) != len(v2):
        raise Exception("dist.euclidean_dist: Unequal vector sizes!")

    ret = 0
    for i in range(len(v1)):
            ret += pow(v1[i] - v2[i], 2)
    
    r = math.sqrt(ret)
    return r




def dist(v1, v2, method, n):
    if method == "Manhattan":
        return manhattan_dist(v1, v2)
    if method == "Euclidean":
        return euclidean_dist(v1, v2)
    elif method == "EMD":
        return emd_dist(v1, v2, n)
    else:
        raise Exception("Dist: Invalid method!")




def cluster_pos(v, n, r):
    
    num_cols = math.sqrt(len(v))
    cluster_size = r*r
    clusters_per_row = num_cols / r
    num_clusters = len(v) / cluster_size

    if (n < 0) or (n >= num_clusters):
        raise Exception("Dist.cluster_pos: Invalid cluster number!")
    
    return (int(r * (n % clusters_per_row)), int(r * (n // clusters_per_row)))




def cluster_sum(v, cluster_pos, r):
    
    assert(is_perfect_square(len(v)))

    ret = 0
    count = r*r
    num_cols = int(math.sqrt(len(v)))
    num_rows = num_cols

    if (cluster_pos[0] < 0) or (cluster_pos[0] >= num_cols) or (cluster_pos[1] < 0) or (cluster_pos[1] >= num_rows):
        raise Exception("Dist.cluster_sum: Invalid cluster position!")

    # v is a 1d vector
    # but represents 2d array
    # v starts counting from upper left corner of the 2d array, that is its (0, 0) position
    # for some reason, we decided that for cluster(i, j) we start counting for bottom left corner
    # thus the (num_rows - j - 1) coefficient

    j = cluster_pos[1]

    while 0 < count:
        i = cluster_pos[0]
        while i < cluster_pos[0] + r:
            ret += v[ (num_rows - j - 1) * num_cols + i ]
            count -= 1
            i += 1
        j = (j+1) % r + cluster_pos[1]

    return ret




def set_up_cluster(v, num_clusters):

    # Assume vector size is a perfect square
    v_size = len(v)
    if is_perfect_square(v_size) is False:
        raise Exception("dist.set_up_cluster error: vector size not a perfect square!")

    # Cluster size must have an integer square root in order for the cluster to be a square
    cluster_size = len(v) / num_clusters
    if is_perfect_square(cluster_size) == 0:
        raise Exception("dist.set_up_cluster error : cluster size not a perfect square!")

    # Size of one dimension of cluster
    # Both dimensions are the same, since a cluster is a square
    r = int(math.sqrt(cluster_size))

    # Compute the sum of each image, for normalization later
    v_sum = sum(v)

    # Each cluster is represented by a tuple t. For example
    # t[0] = (2, 3), its "location"
    # t[1] = 20, its sum of weights
    # We want an array of clusters for each image, so we have
    cluster = []

    for i in range(num_clusters):
        pos = cluster_pos(v, i, r)
        s = cluster_sum(v, pos, r) / v_sum
        cluster.append((pos, s))

    return cluster




def lp_solve(cluster1, cluster2):

    assert(len(cluster1) == len(cluster2))
    
    num_clusters = len(cluster1)

    solver = solver = pywraplp.Solver('EmdSolver',
                           pywraplp.Solver.GLOP_LINEAR_PROGRAMMING)

    # define variables in the form of fij
    # for flow from v1 i-th cluster to v2 j-th cluster
    f = [[0] * num_clusters] * num_clusters
    for i in range(num_clusters):
        for j in range(num_clusters):
            f[i][j] = solver.NumVar(0, solver.infinity(), 'f'+str(i)+str(j))

    # define objective function
    objective = solver.Objective()
    for i in range(num_clusters):
        for j in range(num_clusters):
            objective.SetCoefficient(f[i][j], euclidean_dist(cluster1[i][0], cluster2[j][0]))
    objective.SetMinimization()

    # add consraint that v1 must give as much as it can
    constraints1 = [0] * num_clusters
    for i in range(num_clusters):
        constraints1[i] = solver.Constraint(cluster1[i][1], cluster1[i][1])
        for j in range(num_clusters):
            constraints1[i].SetCoefficient(f[i][j], 1)

    # add consraint that v2 must receive as much as it can
    constraints2 = [0] * num_clusters
    for j in range(num_clusters):
        constraints2[j] = solver.Constraint(cluster2[j][1], cluster2[j][1])
        for i in range(num_clusters):
            constraints2[j].SetCoefficient(f[i][j], 1)

    # print problem
    print(solver.ExportModelAsLpFormat(False).replace('\\', '').replace(',_', ','), sep='\n')

    # solve
    status = solver.Solve()
    if status == solver.OPTIMAL:
        return solver.Objective().Value()
    else:
        raise Exception("dist.lp_solve error: no optimal solution found!")




def emd_dist(v1, v2, num_clusters):

    if num_clusters == 0:
        raise Exception("dist.emd_dist error : Divided by zero !")

    if len(v1) != len(v2):
        raise Exception("dist.emd_dist error : Invalid vector sizes")

    cluster1 = set_up_cluster(v1, num_clusters)
    cluster2 = set_up_cluster(v2, num_clusters)
    return lp_solve(cluster1, cluster2)
        


class Test(unittest.TestCase):
    
    @classmethod
    def setUpClass(self):
        self.v_1x1_1 = [1]
        self.v_2x2_1 = [1, 2, 3, 4]
        self.v_2x2_2 = [3, 4, 1, 2]
        self.v_4x4_1 = [1, 2, 5, 6, 3, 4, 7, 8, 9, 10, 13, 14, 11, 12, 15, 16]
        self.v_4x4_2 = [11, 12, 15, 16, 9, 10, 13, 14, 3, 4, 7, 8, 1, 2, 5, 6]
        self.v_6x6_1 = [1, 2, 3, 4, 5, 6, 2, 3, 4, 5, 6, 1, 3, 4, 5, 6, 1, 2, 4, 5, 6, 1, 2, 3, 5, 6, 1, 2, 3, 4, 6, 1, 2, 3, 4, 5]
        self.v_6x6_2 = [6, 5, 4, 3, 2, 1, 5, 4, 3, 2, 1, 6, 4, 3, 2, 1, 6, 5, 3, 2, 1, 6, 5, 4, 2, 1, 6, 5, 4, 3, 1, 6, 5, 4, 3, 2]


    def test_is_perfect_square(self):
        self.assertTrue(is_perfect_square(1))
        self.assertTrue(is_perfect_square(4))
        self.assertTrue(is_perfect_square(9))
        self.assertTrue(is_perfect_square(196))

        self.assertFalse(is_perfect_square(2))
        self.assertFalse(is_perfect_square(3))
        self.assertFalse(is_perfect_square(8))
        self.assertFalse(is_perfect_square(195))


    def test_manhattan(self):
        self.assertEqual(manhattan_dist(self.v_1x1_1, self.v_1x1_1), 0)
        self.assertEqual(manhattan_dist(self.v_2x2_1, self.v_2x2_2), 8)
        self.assertEqual(manhattan_dist(self.v_4x4_1, self.v_4x4_2), 128)
        
        self.assertRaises(Exception, lambda: manhattan_dist(self.v_2x2_2, self.v_4x4_1))


    def test_euclidean(self):
        self.assertEqual(euclidean_dist(self.v_1x1_1, self.v_1x1_1), 0)
        self.assertEqual(euclidean_dist(self.v_2x2_1, self.v_2x2_2), math.sqrt(16))
        self.assertEqual(euclidean_dist(self.v_4x4_1, self.v_4x4_2), math.sqrt(1088))
        
        self.assertRaises(Exception, lambda: euclidean_dist(self.v_1x1_1, self.v_2x2_1))


    def test_cluster_pos(self):
        self.assertEqual(cluster_pos(self.v_6x6_1, 15, 1), (3, 2))
        self.assertEqual(cluster_pos(self.v_6x6_1, 0, 1), (0, 0))
        self.assertEqual(cluster_pos(self.v_6x6_1, 35, 1), (5, 5))

        self.assertEqual(cluster_pos(self.v_6x6_1, 3, 2), (0, 2))
        self.assertEqual(cluster_pos(self.v_6x6_1, 0, 2), (0, 0))
        self.assertEqual(cluster_pos(self.v_6x6_1, 8, 2), (4, 4))

        self.assertEqual(cluster_pos(self.v_6x6_2, 2, 3), (0, 3))
        self.assertEqual(cluster_pos(self.v_6x6_2, 0, 3), (0, 0))
        self.assertEqual(cluster_pos(self.v_6x6_2, 3, 3), (3, 3))

        self.assertRaises(Exception, lambda: cluster_pos(self.v_6x6_1, 9, 2))
        self.assertRaises(Exception, lambda: cluster_pos(self.v_6x6_1,-1, 2))
        self.assertRaises(Exception, lambda: cluster_pos(self.v_6x6_1, 4, 3))
        self.assertRaises(Exception, lambda: cluster_pos(self.v_6x6_1,-1, 3))


    def test_cluster_sum(self):
        self.assertEqual(cluster_sum(self.v_6x6_2, (3, 2), 1), 6)
        self.assertEqual(cluster_sum(self.v_6x6_2, (0, 0), 1), 1)
        self.assertEqual(cluster_sum(self.v_6x6_2, (5, 5), 1), 1)

        self.assertEqual(cluster_sum(self.v_6x6_2, (0, 2), 2), 12)
        self.assertEqual(cluster_sum(self.v_6x6_2, (0, 0), 2), 10)
        self.assertEqual(cluster_sum(self.v_6x6_2, (4, 4), 2), 10)


    def test_set_up_cluster(self):
        cluster = set_up_cluster(self.v_4x4_1, 4)
        s = sum(self.v_4x4_1)
        correct = [ ((0, 0), 42/s), ((2, 0), 58/s), ((0, 2), 10/s), ((2, 2), 26/s) ]
        self.assertEqual(cluster, correct)

        cluster = set_up_cluster(self.v_4x4_1, 1)
        s = sum(self.v_4x4_1)
        correct = [ ((0, 0), 1) ]
        self.assertEqual(cluster, correct)

        cluster = set_up_cluster(self.v_2x2_2, 4)
        s = sum(self.v_2x2_2)
        correct = [ ((0, 0), 1/s), ((1, 0), 2/s), ((0, 1), 3/s), ((1, 1), 4/s) ]
        self.assertEqual(cluster, correct)

        cluster = set_up_cluster(self.v_6x6_2, 9)
        s = sum(self.v_6x6_2)
        self.assertEqual(cluster[5], ((4, 2), 20/s))

    def test_lp_solve(self):
        v1 = [1, 1, 1, 5]
        v2 = [3, 1, 2, 2]
        c1 = set_up_cluster(v1, len(v1))
        c2 = set_up_cluster(v2, len(v2))
        print("obj_f =", lp_solve(c1, c2))



if __name__ == "__main__":
    unittest.main()