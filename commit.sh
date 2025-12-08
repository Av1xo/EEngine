#!/bin/bash

VERSION_FILE=".git_version"

if [ ! -f "$VERSION_FILE" ]; then
    echo "0.0.0.000" > "$VERSION_FILE"
fi

CURRENT_VERSION=$(cat "$VERSION_FILE")

IFS='.' read -r -a VERSION_PARTS <<< "$CURRENT_VERSION"
MAJOR=${VERSION_PARTS[0]}
MINOR=${VERSION_PARTS[1]}
PATCH=${VERSION_PARTS[2]}
BUILD=${VERSION_PARTS[3]}

BUILD=$((10#$BUILD + 1))


BUILD=$(printf "%03d" $BUILD)

NEW_VERSION="$MAJOR.$MINOR.$PATCH.$BUILD"

echo "Поточна версія: $CURRENT_VERSION"
echo "Нова версія: $NEW_VERSION"
echo ""
read -p "Введіть текст коміту: " COMMIT_MESSAGE

if [ -z "$COMMIT_MESSAGE" ]; then
    echo "Помилка: текст коміту не може бути порожнім!"
    exit 1
fi

git add .

git commit -m "[EverEngine $NEW_VERSION] $COMMIT_MESSAGE"

if [ $? -eq 0 ]; then
    echo "$NEW_VERSION" > "$VERSION_FILE"
    echo ""
    echo "✓ Коміт успішно створено з версією $NEW_VERSION"
else
    echo ""
    echo "✗ Помилка при створенні коміту"
    exit 1
fi