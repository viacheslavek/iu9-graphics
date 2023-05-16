# ----------------------------------------------------------------------------------------------------------------
S = "161 137 429 376 558 192 619 418 281 431"
C = "183 391 224 240 610 107 657 361 429 376"

# resut
# ####################
# 261.867222318,226.95248557436787
# 429.0,376.0
# 262.690140845,386.1408450704268
# 215.627162458,270.8365480205366
# 224,240
# ####################
# 429.0,376.0
# 558,192
# 604.546479034,364.45088953723683
# ####################
# ----------------------------------------------------------------------------------------------------------------
# S = "281 159 472 155 569 248 506 419 242 366"
# C = "149 241 282 72 559 111 628 406 418 475 170 407"

# result
###################
# 281,159
# 472,155
# 569,248
# 506,419
# 242,366
###################
# ----------------------------------------------------------------------------------------------------------------
# S包含C
# S = "259 113 689 107 548 481 103 374"
# C = "306 178 565 160 596 263 441 387 255 326"
#
# ####################
# 306,178
# 565,160
# 596,263
# 441,387
# 255,326
# ####################
# ----------------------------------------------------------------------------------------------------------------
# 完全不相交
# S = "79 176 221 169 341 240 282 430 52 429"
# C = "456 111 680 172 691 287 413 320"

# result
# ####################
# ####################
# ----------------------------------------------------------------------------------------------------------------
# S = "247 183 536 181 536 374 129 374"
# C = "128 71 295 70 455 374 263 374"

# result
# ####################
# 455.0,374.0
# 263.0,374.0
# 206.853653781,247.98264515291112
# 247,183
# 354.08365088,182.258936672
# ####################
# ----------------------------------------------------------------------------------------------------------------
# S = "70 104 227 357 281 219 412 455 625 299 688 484 120 487"
# C = "125 256 689 242 710 427 40 371"

# reverse isClockwise = False
# S = "70 104 120 487 688 484 625 299 412 455 281 219 227 357"
# C = "125 256 40 371 710 427 689 242"

# result
# #######################################################################
# 163.727558838,255.03867761749646
# 227,357
# 267.909855208,252.45259224660992
# 299.13873038,251.6774074019149
# 381.203045416,399.5184634974567
# 105.571884481,376.4806351208
# 95.120741418,296.42487925800003
# 125,256
# ####################
# 476.83944765,407.5119538334328
# 625,299
# 667.375977811,423.4373951603224
# ####################
# ----------------------------------------------------------------------------------------------------------------
# S = "126 121 335 391 575 194 633 442 169 434"
# C = "176 234 622 132 656 362 90 328"

# result
# ####################
# 207.83466678,226.71942598304932
# 295.802997653,340.3627242406396
# 154.973898097,331.9030256807385
# 146.025,266.7633720930233
# 176,234
# ####################
# 389.810382578,346.00981096758306
# 575,194
# 613.696002775,359.4587704847173
# ####################
# ----------------------------------------------------------------------------------------------------------------
# S = "173 106 333 366 663 232 663 457 128 443"
# C = "81 183 737 179 713 379 59 292"

# result
# ####################
# 219.863551402,182.15327102803658
# 307.831667948,325.1014604151009
# 146.607013414,303.6541439862661
# 162.784691161,182.50131285877438
# ####################
# 402.655510288,337.71564127684405
# 663,232
# 663.0,372.34862385321105
# ####################
# ----------------------------------------------------------------------------------------------------------------
# S = "180 115 357 115 378 293 180 293"
# C = "123 115 304 115 304 232 44 232"

# result
# ####################
# 180.0,232.0
# 180.0,115.0
# 304,115
# 304,115.0
# 304,232
# ####################
# ----------------------------------------------------------------------------------------------------------------
# S = "242 78 480 77 480 289 242 289"
# C = "162 110 388 19 386 103 162 247"

# Datastruct
class baseVertex:
    def __init__(self, x, y):
        self.x = x
        self.y = y


class Vertex(baseVertex):
    def __init__(self, x, y, next=None):
        super(Vertex, self).__init__(x, y)
        self.next = next


class Intersection(baseVertex):
    def __init__(self, x, y, nextS=None, nextC=None, crossDi=-1):
        super(Intersection, self).__init__(x, y)
        self.nextS = nextS
        self.nextC = nextC
        self.crossDi = crossDi
        self.used = False


def floatEqual(f1, f2):
    prec = 1e-5
    if abs(f1 - f2) < prec:
        return True
    else:
        return False


def floatLarger(f1, f2):
    if floatEqual(f1, f2):
        return False
    elif f1 > f2:
        return True
    else:
        return False


def isVertexInPolygon(v, list):
    judgeIndex = 0
    for i in range(len(list)):
        j = i + 1
        minY = min(list[i % len(list)].y, list[j % len(list)].y)
        maxY = max(list[i % len(list)].y, list[j % len(list)].y)
        if floatLarger(v.y, maxY) or floatLarger(minY, v.y):
            continue
        if floatEqual(maxY, minY):
            if floatLarger(v.x, max(list[i % len(list)].x, list[j % len(list)].x)):
                judgeIndex += 1
                continue
            elif floatLarger(min(list[i % len(list)].x, list[j % len(list)].x), v.x):
                continue
            else:
                return True

        x = (list[i % len(list)].x - list[j % len(list)].x) / (list[i % len(list)].y - list[j % len(list)].y) * (
                v.y - list[i % len(list)].y) + list[i % len(list)].x
        if (floatEqual(v.x, x)):
            return None
        if floatLarger(v.x, x):
            judgeIndex += 1
    if judgeIndex % 2 != 0:
        return True
    return False


def getX(v):
    return v.x


def getY(v):
    return v.y


def LineCrossH(y, c1, c2):
    return c1.x + (c2.x - c1.x) * (y - c1.y) / (c2.y - c1.y)


def LineCrossV(x, c1, c2):
    return c1.y + (c2.y - c1.y) * (x - c1.x) / (c2.x - c1.x)


def CutByVerticalLine(s1, s2, list):
    assert floatEqual(s1.x, s2.x)
    crossXs = []
    x = s1.x

    shearedList = [Vertex(r.x, r.y) for r in list]

    minY = min(s1.y, s2.y)
    maxY = max(s1.y, s2.y)

    for i in range(len(list)):
        vertex = list[i]
        c1 = shearedList[i % len(list)]
        c2 = shearedList[(i + 1) % len(list)]

        if (floatEqual(c1.x, c2.x) and floatEqual(c1.x, x)):
            continue
        if (floatLarger(c1.x, x) and floatLarger(c2.x, x)):
            continue
        if (floatLarger(x, c1.x) and floatLarger(x, c2.x)):
            continue

        y = float('%.9f' % LineCrossV(x, c1, c2))

        inters = Intersection(x, y)

        next = None
        if ((floatLarger(y, minY) and floatLarger(maxY, y))
                # or (c2.y == y and x == s2.x)
                # or (c1.y == y and x == s1.x)
                or (floatEqual(c2.x, x) and floatEqual(y, s1.y))
                or (floatEqual(c1.x, x) and floatEqual(y, s2.y))
                or (floatEqual(y, minY) and (not floatEqual(c1.x, x)) and (not floatEqual(c2.x, x)))
                or (floatEqual(y, maxY) and (not floatEqual(c1.x, x)) and (not floatEqual(c2.x, x)))):
            while not ((isinstance(vertex, Vertex) and isinstance(vertex.next, Vertex)) or (
                    isinstance(vertex, Intersection) and isinstance(vertex.nextS, Vertex))):
                if isinstance(vertex, Vertex):
                    assert isinstance(vertex.next, Intersection)
                    if (floatLarger(c2.x, c1.x) and floatLarger(vertex.next.x, inters.x)) or (
                            floatLarger(c1.x, c2.x) and floatLarger(inters.x,
                                                                    vertex.next.x)):
                        break
                    vertex = vertex.next
                else:
                    assert isinstance(vertex.nextS, Intersection)
                    if (floatLarger(c2.x, c1.x) and floatLarger(vertex.nextS.x, inters.x)) or (
                            floatLarger(c1.x, c2.x) and floatLarger(inters.x, vertex.nextS.x)):
                        break
                    vertex = vertex.nextS
            if isinstance(vertex, Vertex):
                next = vertex.next
            else:
                next = vertex.nextS
            if isinstance(vertex, Vertex):
                vertex.next = inters
            else:
                assert isinstance(vertex, Intersection)
                vertex.nextS = inters
            inters.nextS = next

            if floatEqual(c1.x, x):
                assert not floatEqual(c2.x, x)
                if floatLarger(c2.x, x):
                    inters.crossDi = 0
                else:
                    inters.crossDi = 1
            elif floatLarger(c1.x, x):
                inters.crossDi = 1
            else:
                inters.crossDi = 0
            if floatLarger(s2.y, s1.y):
                inters.crossDi = 0 if inters.crossDi == 1 else 1

            # print("s1:%s, s2:%s, c1:%s, c2:%s, inter:%s, crossDi:%s" % (("%f, %f" % (s1.x, s1.y)), ("%f, %f" % (s2.x, s2.y)), ("%f, %f" % (c1.x, c1.y)), ("%f, %f" % (c2.x, c2.y)), ("%f, %f" % (inters.x, inters.y)), ("%s" % ("in" if inters.crossDi == 0 else "out"))))
            crossXs.append(inters)
    return crossXs


def CutByLine(s1, s2, list):
    # print("s1 = %s, s2 = %s" % (("%f, %f" % (s1.x, s1.y)), ("%f, %f" % (s2.x, s2.y))))

    if floatEqual(s1.x, s2.x):
        return CutByVerticalLine(s1, s2, list)
    crossXs = []

    slope = (s2.y - s1.y) / (s1.x - s2.x)
    y = s1.x * slope + s1.y
    shearedList = [Vertex(r.x, r.x * slope + r.y) for r in list]

    minX = min(s1.x, s2.x)
    maxX = max(s1.x, s2.x)

    for i in range(len(list)):
        vertex = list[i]
        c1 = shearedList[i % len(list)]
        c2 = shearedList[(i + 1) % len(list)]
        # print("c1 = %s, c2 = %s" % (("%f, %f" % (c1.x, c1.y - c1.x * slope)), ("%f, %f" % (c2.x, c2.y - c2.x * slope))))

        if (floatEqual(c1.y, c2.y) and floatEqual(c1.y, y)):
            continue
        if (floatLarger(c1.y, y) and floatLarger(c2.y, y)):
            continue
        if (floatLarger(y, c1.y) and floatLarger(y, c2.y)):
            continue

        x = float('%.9f' % LineCrossH(y, c1, c2))
        npy = y - x * slope
        inters = Intersection(x, npy)

        next = None
        if ((floatLarger(x, minX) and floatLarger(maxX, x))
                # or (c2.y == y and x == s2.x)
                # or (c1.y == y and x == s1.x)
                or (floatEqual(c2.y, y) and floatEqual(x, s1.x))
                or (floatEqual(c1.y, y) and floatEqual(x, s2.x))
                or (floatEqual(x, minX) and (not floatEqual(c1.y, y)) and (not floatEqual(c2.y, y)))
                or (floatEqual(x, maxX) and (not floatEqual(c1.y, y)) and (not floatEqual(c2.y, y)))):

            while not ((isinstance(vertex, Vertex) and isinstance(vertex.next, Vertex)) or (
                    isinstance(vertex, Intersection) and isinstance(vertex.nextS, Vertex))):
                if isinstance(vertex, Vertex):
                    assert isinstance(vertex.next, Intersection)

                    if (floatLarger(c2.x, c1.x) and floatLarger(vertex.next.x, inters.x)) \
                            or (floatLarger(c1.x, c2.x) and floatLarger(inters.x, vertex.next.x)) \
                            or (floatLarger(c1.y - c1.x * slope, c2.y - c2.x * slope) and floatLarger(inters.y,
                                                                                                      vertex.next.y)) \
                            or (floatLarger(c2.y - c2.x * slope, c1.y - c1.x * slope) and floatLarger(vertex.next.y,
                                                                                                      inters.y)):
                        break
                    vertex = vertex.next
                else:
                    assert isinstance(vertex.nextS, Intersection)
                    if (floatLarger(c2.x, c1.x) and floatLarger(vertex.nextS.x, inters.x)) \
                            or (floatLarger(c1.x, c2.x) and floatLarger(inters.x, vertex.nextS.x)) \
                            or (floatLarger(c2.y - c2.x * slope, c1.y - c1.x * slope) and floatLarger(inters.y,
                                                                                                      vertex.nextS.y)) \
                            or (floatLarger(c2.y - c2.x * slope, c1.y - c1.x * slope) and floatLarger(vertex.nextS.y,
                                                                                                      inters.y)):
                        break
                    vertex = vertex.nextS
            if isinstance(vertex, Vertex):
                next = vertex.next
            else:
                next = vertex.nextS
            if isinstance(vertex, Vertex):
                vertex.next = inters
            else:
                assert isinstance(vertex, Intersection)
                vertex.nextS = inters
            inters.nextS = next

            if floatEqual(c1.y, y):
                assert not floatEqual(c2.y, y)
                if floatLarger(y, c2.y):
                    inters.crossDi = 0
                else:
                    inters.crossDi = 1
            elif floatLarger(y, c1.y):
                inters.crossDi = 1
            else:
                inters.crossDi = 0

            if floatLarger(s2.x, s1.x):
                inters.crossDi = 0 if inters.crossDi == 1 else 1

            # print("s1:%s, s2:%s, c1:%s, c2:%s, inter:%s, crossDi:%s" % (("%f, %f" % (s1.x, s1.y)), ("%f, %f" % (s2.x, s2.y)), ("%f, %f" % (c1.x, c1.y - c1.x * slope)), ("%f, %f" % (c2.x, c2.y - c2.x * slope)), ("%f, %f" % (inters.x, inters.y)), ("%s" % ("in" if inters.crossDi == 0 else "out"))))
            crossXs.append(inters)

    return crossXs


def processNoCross(listS, listC):
    sInC = isVertexInPolygon(listS[0], listC)
    if sInC:
        return listS
    cInS = isVertexInPolygon(listC[0], listS)
    if cInS:
        return listC
    return []


def printList(start, isS):
    assert isinstance(start, Vertex)
    next = start.next
    print("#######################################################################")
    if isS:
        print("list S:")
        print(str(start.x) + "," + str(start.y))
        while next != start:
            print(str(next.x) + "," + str(next.y))
            if isinstance(next, Vertex):
                next = next.next
            else:
                assert isinstance(next, Intersection)
                print(next.crossDi)
                next = next.nextS
    else:
        print("list C:")
        print(str(start.x) + "," + str(start.y))
        while next != start:
            print(str(next.x) + "," + str(
                next.y))
            if isinstance(next, Vertex):
                next = next.next
            else:
                assert isinstance(next, Intersection)
                print(next.crossDi)
                next = next.nextC
    print("#######################################################################")


def Compose(list):
    result = []
    for inters in list:
        assert isinstance(inters, Intersection)
        if (not inters.used) and inters.crossDi == 0:
            oneResult = []
            oneResult.append(Vertex(inters.x, inters.y))
            inters.used = True
            loopvar = inters.nextS
            # print("--------------------" + str(inters.x) + "," + str(inters.y))
            while loopvar != None:
                # print(str(loopvar.x) + "," + str(loopvar.y))
                oneResult.append(Vertex(loopvar.x, loopvar.y))
                if isinstance(loopvar, Intersection):
                    curr = loopvar
                    curr.used = True
                    next = curr.nextS if curr.crossDi == 0 else curr.nextC
                elif isinstance(loopvar, Vertex):
                    curr = loopvar
                    next = curr.next
                if next is inters:
                    break
                loopvar = next
            result.append(oneResult)
    for vertexs in result:
        for i in range(len(vertexs)):
            if i >= len(vertexs):
                break
            u = vertexs[i % len(vertexs)]
            v = vertexs[(i + 1) % len(vertexs)]
            if (floatEqual(u.x, v.x) and floatEqual(u.y, v.y)):
                vertexs.pop(i)
            i -= 1
    return result


def decode(lists):
    results = []
    for list in lists:
        result = ""
        for v in list:
            result += "%f %f " % (v.x, v.y)
        result = result.strip()
        results.append(result)
    return results


def encode(Str):
    myList = []
    list_float = list(map(float, Str.strip().split()))
    X = list_float[0::2]
    Y = list_float[1::2]
    assert len(X) == len(Y)
    for i in range(len(X)):
        if (not floatEqual(X[i], X[i - 1])) or (not floatEqual(Y[i], Y[i - 1])):
            myList.append(Vertex(X[i], Y[i]))
    return myList


def transDirect(list):
    newList = []
    for i in range(len(list)):
        newList.append(list[len(list) - 1 - i])
    return newList


def toClockwise(list):
    crossPr = []
    maxX = -1
    mark_i = -1

    for i in range(len(list)):
        if list[i].x > maxX:
            maxX = list[i].x
            mark_i = i
    v1 = Vertex(list[mark_i].x - list[mark_i - 1].x, list[mark_i].y - list[mark_i - 1].y)
    v2 = Vertex(list[(mark_i + 1) % len(list)].x - list[mark_i].x, list[(mark_i + 1) % len(list)].y - list[mark_i].y)
    crossPr = v1.x * v2.y - v2.x * v1.y
    while floatEqual(crossPr, 0):
        mark_i += 1
        v2 = Vertex(list[(mark_i + 1) % len(list)].x - list[mark_i % len(list)].x,
                    list[(mark_i + 1) % len(list)].y - list[mark_i % len(list)].y)
        crossPr = v1.x * v2.y - v2.x * v1.y
    assert not floatEqual(crossPr, 0)
    if crossPr < 0:
        return transDirect(list)
    else:
        return list


def PolyClipping(S, C, output_clockwise=True):

    listS = encode(S)
    listC = encode(C)
    listS = toClockwise(listS)
    listC = toClockwise(listC)
    listI = []

    for i in range(len(listS)):
        listS[i - 1].next = listS[i]
    for i in range(len(listC)):
        listC[i - 1].next = listC[i]

    for cutStartIdx in range(len(listC)):
        s1 = listC[cutStartIdx]
        s2 = listC[(cutStartIdx + 1) % len(listC)]

        inters = CutByLine(s1, s2, listS)
        if len(inters) == 0:
            continue

        if floatEqual(s1.x, s2.x):
            assert not floatEqual(s1.y, s2.y)
            if floatLarger(s2.y, s1.y):
                inters.sort(key=getY)
            else:
                inters.sort(key=getY, reverse=True)
        elif floatLarger(s2.x, s1.x):
            inters.sort(key=getX)
        else:
            inters.sort(key=getX, reverse=True)

        for v in inters:
            listI.append(v)

        s1.next = inters[0]
        for i in range(len(inters) - 1):
            inters[i].nextC = inters[i + 1]
        inters[len(inters) - 1].nextC = s2

    if len(listI) == 0:
        return decode([processNoCross(listS, listC)])

    results = Compose(listI)
    if not output_clockwise:
        results_ = []
        for result in results:
            result = transDirect(result)
            results_.append(result)
        results = results_
    return decode(results)

# USAGE
result = PolyClipping(S, C, False)
for r in result:
    print(r)
