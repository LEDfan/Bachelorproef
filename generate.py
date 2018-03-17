#!/usr/bin/env python3
import json
from sys import argv

def add(testResults, branch, build_id, name, href):
    if branch not in testResults:
        testResults[branch] = []

    testResults[branch].append(
        {
            "href": href,
            "name": name,
            "build_id": build_id
        }
    )

    testResults[branch].sort(key=lambda el: el['build_id'].split('.')[0], reverse=True)
    testResults[branch].sort(key=lambda el: el['build_id'].split('.')[1])

def renderBranch(name, builds):
    print("# " + name)
    for build in builds:
        print(" - [" + build['build_id'] + " " + build['name'] + "](" + build['href'] + ")")


def render(testResults):
    print("Test results")
    print("===")
    if 'master' in testResults:
        renderBranch("master", testResults['master'])

    for branch, builds in testResults.items():
        if branch != 'master':
            renderBranch(branch, builds)

def save(testResults):
    with open('test_results.json', 'w') as file:
        json.dump(testResults, file, indent=2, ensure_ascii=False)

if __name__ == "__main__":

    with open('test_results.json') as file:
        testResults = json.load(file)

    if argv[1] == "add":
        # branch, build_id, name, href
        add(testResults, argv[2], argv[3], argv[4], argv[5])
        save(testResults)
    elif argv[1] == "render":
        render(testResults)
