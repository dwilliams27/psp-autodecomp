# ADR-015: psp-autodecomp runs as a subscription-only sink sub-tenant on the mini

**Status:** Accepted (design). Awaiting first supervised deploy on the mini.
**Date:** 2026-06-27
**Author:** Claude Opus 4.8
**Relates to:** the assistant repo's `docs/projects/psp-decomp-tenant.md` (substrate side)
and `sink` decision 0002 (subscription-compute-only), which this mirrors.

## Context

psp-autodecomp's overnight harness (`tools/orchestrator.py` via `tools/run_overnight.sh`)
was built to run on a developer Mac: it creates a `dscl` sandbox user `autodecomp`, a PF
egress firewall, unlocks that user's keychain for Claude auth, and tolerates an
`OPENAI_API_KEY` for the Codex backend. David wants these runs to happen **unattended,
overnight, on the always-on mini** — the same machine that hosts the `sink` compute-sink
tenant and his hardened personal assistant, under kernel uid-isolation and a single
household kill switch.

The mini's tenancy rules (defined in David's private machine-config repo, *not* here) are
strict: subscription compute only (no metered API keys), one fine-grained PAT per tenant,
a shared `/var/db/household-halt` kill switch, and uid isolation as the trust boundary.

## Decision

Run psp-autodecomp on the mini as a **sink sub-tenant**: a sibling of the `sink` tenant,
sharing only the hardware and the kill switch.

1. **Its own nix-declared uid** (`autodecomp`, uid 573) — *not* folded into `sink`'s
   `_sink` uid. psp is a private repo with its own PAT and carries the game ISO +
   toolchain; co-locating it under `_sink` (which runs public-repo sessions on the open
   web) would expose psp's private PAT across a trust boundary. The account keeps the name
   `autodecomp` (our scripts/ACLs use it) but is created by nix, not `dscl`.

2. **Subscription compute only.** A new entrypoint, **`tools/run_sink.sh`**, is the
   workload half. It **refuses to start if `OPENAI_API_KEY` is set** and uses subscription
   OAuth (`~/.claude`, `~/.codex`); like `run_overnight.sh` it unlocks the login keychain
   best-effort in case Claude Code stored its OAuth there (the headless-auth path for a
   never-GUI-login service account must be verified on first bring-up — see the deploy
   checklist's auth gate). The hard line `run_overnight.sh` does not draw is the
   `OPENAI_API_KEY` refusal (it preserves the key through sudo); `run_overnight.sh` is
   unchanged and remains the laptop path.

3. **The privileged step stays out of the agent's reach.** The repo checkout on the mini
   (`/Users/autodecomp/psp-autodecomp`) is autodecomp-writable, so an agent could rewrite
   any script in it. The one root action — bringing up the PF egress jail — therefore lives
   in the nix-store `psp-run` command (root-owned), and the **jail RULES are nix-managed**
   (`environment.etc."pf.anchors/autodecomp"`), *not* in the checkout — so the agent can't
   neuter its own jail. `sandbox_setup.sh` (which creates a user + writes PF rules from the
   checkout) is **laptop-only and refuses to run on the mini**. The only thing exec'd from
   the checkout, `run_sink.sh`, runs strictly **as autodecomp**, inside the uid boundary the
   agent already controls. No path lets a compromised agent get root. (Caveat recorded in
   the threat model: the agenix PAT is readable by the autodecomp uid by design and the jail
   permits 443 egress, so a compromised agent *can* exfiltrate the PAT — this is why the PAT
   is single-repo, contents-only, short-expiry; same posture as `_sink`'s token.)

4. **Kill switch.** `psp-run` halt-gates on `/var/db/household-halt`; a live run is reaped
   because uid 573 is in `household-halt`'s uid-reap loop. One button stops the orchestrator
   and every agent child.

5. **Git posture.** The orchestrator's existing behavior is unchanged: clean-`main`-only,
   branch to `overnight/<ts>`, commit locally, **no push**. `run_sink.sh` adds an opt-in
   `--push` to back the overnight branch up to origin for off-box review; default is local.

## Consequences

- Overnight psp runs on the mini are launched with `psp-run 8h` and stopped with
  `sudo household-halt` — identical ergonomics to `sink-run`.
- The Codex backend on the mini must use subscription auth (`codex login`), never a key.
- `run_sink.sh` duplicates a little of `run_overnight.sh`'s preflight logic rather than
  sharing it; acceptable for now, but if the preflights drift the two should be refactored
  to a common helper. Noted as a follow-up.
- Management-page (approval page) start/stop for psp is **not** in this change — Phase 2.
  Today psp is launched over SSH and stopped by the existing "Stop EVERYTHING" button.

## Alternatives considered

- **Fold psp into `_sink`** — rejected: cross-tenant exposure of the private PAT + ISO.
- **Point sink's generic clone+prompt loop at the psp repo** — rejected: it would discard
  the DB-driven orchestrator (the actual value), and psp's deps (ISO, `extern/`) are
  git-ignored, so a fresh clone cannot build.
- **Run `run_overnight.sh` as-is under nix** — rejected: it allows the `OPENAI_API_KEY`
  metered path and would have a root-capable operator execute an agent-writable script.
