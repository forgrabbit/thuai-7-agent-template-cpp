# syntax=docker/dockerfile:1

FROM gcc:12.3.0 AS build-env
WORKDIR /app
RUN curl -fsSL https://xmake.io/shget.text | bash
RUN curl https://www.google.com/generate_204 \
 || ~/.local/bin/xmake g --proxy_pac=github_mirror.lua --root
COPY . .
RUN ~/.local/bin/xmake f -m release -v -y --root \
 && ~/.local/bin/xmake -v --root

FROM gcr.nju.edu.cn/distroless/cc-debian12
WORKDIR /app
COPY --from=build-env /app/bin/agent .
ENTRYPOINT ["./agent"]
