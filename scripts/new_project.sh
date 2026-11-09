#!/bin/bash

if [[ $# -ne 1 || ! "$1" =~ ^[A-Za-z0-9][A-Za-z0-9_-]*$ ]]; then
    printf 'Usage: %s <project_name>\n' "$0" >&2
    exit 2
fi

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
projects_dir="$script_dir/../projects"
template_dir="./scripts/_project_template"
project_dir="$projects_dir/$1"

if [[ ! -d "$template_dir" ]]; then
    printf 'Project template not found: %s\n' "$template_dir" >&2
    exit 1
fi

if [[ -e "$project_dir" || -L "$project_dir" ]]; then
    printf 'Project already exists: %s\n' "$project_dir" >&2
    exit 1
fi

cp -R "$template_dir" "$project_dir"
printf 'Created project: %s\n' "$project_dir"
