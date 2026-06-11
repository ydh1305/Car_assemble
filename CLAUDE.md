# Car Assemble Project — Claude 작업 규칙

## 필수 참조 문서

| 문서 | 목적 |
|------|------|
| `temp_docs/spec.md` | 불변 요구사항 명세 — 모든 코드 변경 시 준수 필수 |
| `temp_docs/PLAN.md` | 리팩토링 단계별 계획 |
| `docs/commit_convention.md` | 커밋 메시지 작성 규칙 — 모든 커밋 시 준수 필수 |

## 코드 변경 전 체크리스트

코드를 수정하거나 새 파일을 작성하기 전에 반드시 `temp_docs/spec.md`를 확인하고,
아래 항목이 변경 후에도 모두 유지되는지 검증한다.

1. 차량 타입 3종 (Sedan / SUV / Truck) 선택 동작
2. 엔진 3종 + 생략(선택 안 함) 옵션 동작
3. 브레이크 시스템 3종 선택 동작
4. 스티어링 시스템 2종 선택 동작
5. 조합 유효성 규칙 5가지 (spec.md §3 Rule 1~2) 모두 FAIL 처리
6. 뒤로 가기 / 처음으로 / RUN / Test / exit 인터랙션 동작
7. FAIL 시 사유 출력

## 리팩토링 원칙

- Phase 1(method level) 완료 후 Phase 2(class level) 진입
- 각 단계는 독립적으로 빌드 가능해야 한다
- 기능 동작은 리팩토링 전후 동일해야 한다 (spec.md 기준)
- 전역 변수(`stack[]`) 제거 방향으로 진행
- 테스트 가능한 구조(CarValidator 등)로 분리
