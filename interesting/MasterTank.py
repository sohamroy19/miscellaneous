def findMasterTank(tanks, pipes):
    n = len(tanks)
    supplied_by = [set() for i in range(n)]

    for pipe in pipes:
        supplied_by[pipe[0] - 1].add(pipe[1])

    modified = True

    while modified:
        modified = False

        for i in range(n):
            to_add = set()

            for d in supplied_by[i]:
                for d_of_d in supplied_by[d - 1]:
                    if d_of_d not in supplied_by[i]:
                        to_add.add(d_of_d)

                        modified = True

            supplied_by[i].update(to_add)
            if len(supplied_by[i]) == n - 1:
                return i + 1

    return 0
