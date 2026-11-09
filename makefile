PROJECT ?=

check-project:
	@test -n "$(PROJECT)" || { printf 'Usage: make <status|build|deploy> PROJECT=<project>\n' >&2; exit 2; }

status:
	./scripts/status.sh

build: check-project
	./scripts/build.sh "$(PROJECT)"

deploy: check-project
	./scripts/deploy.sh "$(PROJECT)"

new-project:
	./scripts/new_project.sh "$(PROJECT)"
