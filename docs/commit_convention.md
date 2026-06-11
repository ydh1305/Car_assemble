# Commit Convention

## 형식

```
<type>(<scope>): <subject>

[body]

[footer]
```

- **type**, **scope**, **subject** 는 필수
- **body**, **footer** 는 선택 (필요한 경우에만 작성)

---

## Type

| type | 설명 |
|------|------|
| `feat` | 새로운 기능 추가 |
| `fix` | 버그 수정 |
| `refactor` | 기능 변경 없는 코드 구조 개선 |
| `test` | 테스트 코드 추가 또는 수정 |
| `docs` | 문서 작성 또는 수정 |
| `chore` | 빌드 설정, 패키지, 기타 잡무 |
| `style` | 코드 포맷·공백 등 로직 무관 변경 |
| `perf` | 성능 개선 |
| `ci` | CI/CD 파이프라인 설정 변경 |

---

## Scope (이 프로젝트 기준)

| scope | 대상 |
|-------|------|
| `util` | Util.h — 열거형, 범위 검사 |
| `part` | 부품 인터페이스 및 구현 클래스 (ICarType, IEngine 등) |
| `rule` | 호환성 규칙 (Rules/) |
| `validator` | CarValidator |
| `registry` | PartRegistry |
| `assembler` | CarAssembler (상태 머신) |
| `printer` | CarPrinter / ICarPrinter |
| `test` | Tests/ 디렉토리 전반 |
| `build` | vcxproj, packages.config, NuGet 설정 |
| `docs` | 문서 파일 |

---

## Subject 작성 규칙

- 영문 기준 50자 이하, 마침표 없음
- 명령형 현재 시제 동사로 시작 (add / fix / remove / update / rename …)
- 한국어 작성도 허용하나 팀 내 통일 권장

```
# 좋은 예
feat(part): add NoEngine class for optional engine selection
fix(util): remove obsolete part-type enums causing name conflict
refactor(assembler): extract processSelection into selectPart

# 나쁜 예
Fixed some bugs.
수정했음
feat: 여러 가지 변경사항 추가
```

---

## Body

- 한 줄에 72자 이하
- **무엇을** 바꿨는지보다 **왜** 바꿨는지 설명
- 글머리 기호(`-`) 사용 가능

```
refactor(validator): inject rules via constructor for testability

Default constructor creates 5 rules internally; overloaded constructor
accepts an external vector so unit tests can inject mocks without
touching the real rule implementations.
```

---

## Footer

| 키워드 | 용도 |
|--------|------|
| `Closes #n` | 이슈 자동 닫기 |
| `Fixes #n` | 버그 이슈 자동 닫기 |
| `BREAKING CHANGE:` | 하위 호환성 파괴 변경 명시 |
| `Refs #n` | 관련 이슈 참조 (닫지 않음) |

---

## 전체 예시

```
feat(rule): add BoschMismatchRule for steering compatibility

Bosch brake requires Bosch steering — mismatched combination must fail
validation per spec §3 Rule 5.

Closes #12
```

```
fix(util): remove obsolete CarType/Engine/brakeSystem/SteeringSystem enums

SUV enumerator (value=2) conflicted with class SUV in the same namespace,
causing C2365 compilation errors in Phase3_CarValidatorTests.cpp.
These enums are unused after OCP refactoring in Phase 3.
```

---

## 금지 사항

- `Co-Authored-By: Claude` 등 AI 공동 저자 표기 금지
- 하나의 커밋에 여러 관심사 혼합 금지 (atomic commit 원칙)
- WIP·임시 커밋을 `master`/`main` 브랜치에 직접 push 금지
