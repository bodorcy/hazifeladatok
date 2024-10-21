def szetvalaszt(szo:str):
    mgh = {'a', 'e', 'i', 'o', 'ö', 'u', 'ü', 'á', 'é', 'í', 'ó', 'ő', 'ú', 'ű'}
    tagolt = list()
    tag_eleje = 0
    while szo[tag_eleje] not in mgh:
        tag_eleje += 1
    tagolt.append(szo[0:tag_eleje+1])
    tag_eleje += 1

    for tag_vege in range(tag_eleje,len(szo)):
        if szo[tag_vege] in mgh:
            tagolt.append(szo[tag_eleje:tag_vege+1])
            tag_eleje = tag_vege + 1
        if tag_vege == len(szo) - 1:
            tagolt[-1] += szo[tag_eleje:tag_vege+1]

    for i in range(1,len(tagolt)):
        if len(tagolt[i]) > 1:
            while tagolt[i][0] not in mgh and tagolt[i][1] not in mgh:
                tagolt[i-1] += tagolt[i][0]
                tagolt[i] = tagolt[i][1:]

    return "-".join(tagolt)


print(szetvalaszt("lalabamam"))
print(szetvalaszt("alma"))
print(szetvalaszt("abba"))
print(szetvalaszt("lead"))
print(szetvalaszt("amper"))
print(szetvalaszt("centrum"))
