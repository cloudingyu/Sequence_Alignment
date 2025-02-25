**Gary Benson**

  

## Abstract

  

Algorithm development for comparing and aligning biological sequences has, until recently, been based on the SI model of mutational events which assumes that modification of sequences proceeds through any of the operations of substitution, insertion, or deletion (the latter two collectively termed indels). While this model has worked fairly well, it has long been apparent that other mutational events occur. In this paper, we introduce a new model, the DSI model, which includes another common mutational event, tandem duplication. Tandem duplication produces tandem repeats which are common in DNA, making up perhaps 10% of the human genome. They are responsible for some human diseases and may serve a multitude of functions in DNA regulation and evolution.

  

Using the DSI model, we develop new exact and heuristic algorithms for comparing and aligning DNA sequences when they contain tandem repeats.

  

---

  

## Introduction

  

Much research has been devoted to developing efficient algorithms for determining the similarity of two strings. An obvious motivation for this work comes from molecular biology. DNA sequences that are found to be similar often share a common origin and have diverged through a series of evolutionary events. The recognition of similar sequences can reveal structural and functional properties of major importance.

  

The method used to compute similarity should mirror the set of mutational events by which sequences can diverge. Until recently, the most common model of mutational events has allowed just three operations to transform one sequence into another. These are substitutions (whereby a single nucleotide base—one of the four building blocks A, C, G, T of DNA—is replaced by a different base), and insertions and deletions (collectively termed indels, whereby a new sequence of bases is inserted between two preexisting bases or a preexisting subsequence is removed and the gap closed). We will refer to this as the SI model for substitutions and indels. The SI model reflects frequent mutational events and within this framework, dynamic programming algorithms are used to compute an optimal similarity score for two sequences and to produce an alignment of their characters.

  

The SI model has worked fairly well, yet, it has long been apparent that there is a panoply of other mutational events that occur, including:

  

1. Inversions — the removal of a subsequence of DNA and the replacement by its reversed sequence.

  

2. Translocations — the excision (removal) of a subsequence of DNA and its insertion in another location.

  

3. Transpositions — the movement or copying of a subsequence of DNA into another location, often facilitated by structures within the subsequence.

  

4. Tandem duplications — the copying of a subsequence of DNA to a position immediately adjacent to the original copy.

  

With ever more DNA sequence data becoming available for analysis, the need to accurately compare sequences which have clearly undergone more complicated types of mutational processes is becoming critical. It is important, therefore, that algorithms be developed that determine similarity and produce alignments in terms of a more complete set of mutational operations.

  

In this paper, we address tandem duplication. Tandem duplication is a mutational process in which a stretch of DNA is duplicated to produce one or more new copies, each copy following the preceding one in a contiguous fashion. For example:

  

$$

\text{ABC} \rightarrow \text{ABBBC}

$$

  

(Here each letter represents a fixed but unspecified number of bases). The result of a tandem duplication is a tandem repeat. Over time, tandem repeats undergo additional mutations so that typically, only approximate copies are present. Tandem repeats occur frequently (comprising perhaps 10% or more of the human genome) and form major chromosomal structures including centromeres and telomeres. They have recently been implicated in the causation of at least eight inherited human diseases including fragile-X mental retardation, Huntington's disease, myotonic dystrophy and Friedreich's ataxia. Tandem repeats may play a significant role in gene regulation, DNA-protein binding, and evolution.

  

Besides their importance in DNA function and expression, tandem repeats are useful laboratory tools. The number of copies of the pattern in a tandem repeat is often variable among individuals (polymorphic). Polymorphic sites are useful in localizing genes to specific regions of the chromosome (linkage studies) and in DNA fingerprinting. Recently, polymorphic tandem repeats have been used to support the "Out of Africa" hypothesis of human evolution and migration and to suggest the evolutionary history of a microsatellite locus in primates.

  

In this paper, we formalize a new model of mutation, the DSI model, for tandem duplications, substitutions and indels. We present the first algorithms for computing sequence similarity and alignment when tandem duplication is allowed as a mutational event. We give both exact and heuristic algorithms.

  

Earlier research on tandem repeats has focused on 1) producing alignments of tandem repeats with a known pattern (wraparound dynamic programming- WDP), and 2) detecting unknown tandem repeats. WDP aligns an unknown number of tandem copies of a pattern sequence B with a sequence A. This is not the same as producing a local alignment of two sequences either of which may contain tandem repeats as subsequences. WDP is described more fully in Appendix A. Algorithms for detecting tandem repeats can be divided into exact algorithms and heuristic algorithms. The exact algorithms search either for tandem repeats or for highest scoring non-overlapping aligned regions (a broader category of repeat). Scoring methods include Hamming distance (substitutions only), unit cost edit distance (substitutions and indels have equal cost), and arbitrarily weighted edit operations. None of the exact algorithms can use the common affine gap penalty scheme (section 2). The best time for the exact algorithms is O (n^2 log n) for sequences of length n.

  

The heuristic algorithms search either for tandem repeats using matching k-tuples, or for "simple sequences" (again, a broader category) with data compression techniques. We make use of the newest algorithm that searches specifically for tandem repeats in our heuristic approach.

  

The remainder of this paper is organized as follows. In section 2 we formalize the DSI model and give an overview of the problem we address. In section 3 we develop an exact algorithm for alignment with tandem repeats. Finally, in section 4 we present a heuristic algorithm which is designed to align DNA sequences containing tandem repeats in an efficient, yet biologically meaningful way. Two examples are presented in section 5.

  

---

  

## Local Alignment under the SI and DSI Models

  

In what follows, we will examine alignment using affine gap penalties. In this formulation, an indel of length $k$ has an associated cost $c(k) = \alpha + k\beta$, where $\alpha$ is the cost for opening a gap and $\beta$ is the cost for extending the gap by one character. ($\alpha$ and $\beta$ are negative values.)

  

An alignment is an optimal pairing up of the characters from two sequences based on a scoring function. In global alignment, the entirety of both sequences must be aligned. In local alignment, the best scoring alignment of any pair of subsequences is determined. In what follows, we are concerned with local alignment.

  

For sequences $S_1$ and $S_2$, under the SI model, with affine gap penalties, local alignment score $S[i,j]$, the optimal score when aligning suffixes of $S_1[1\cdots i]$ and $S_2[1\cdots j]$, is the maximum of four values:

  

$$

S[i,j] = \max 
\begin{cases}
&E[i,j]\\

&F[i,j]\\

&M[i,j]\\

&0
\end{cases}

$$

  

Where:

  

- $E(i,j)$ is the highest score given that the alignment ends with a deletion at the right end of $S_2[1\cdots j]$,

  

- $F(i,j)$ is the highest score given that the alignment ends with a deletion at the right end of $S_1[1\cdots i]$,

  

- $M[i, j]$ is the highest score given that the alignment ends with a match (or substitution) between $S_1[i]$ and $S_2[j]$,

  

- Zero allows the suffixes that participate in the alignment to be optimally selected.

  

For the DSI model, we add another option, the duplication option. Thus, $\text{Dup}[i,j]$ is the highest score given that the alignment ends with a duplication of the right end of $S_1[1\cdots i]$ or a duplication of the right end of $S_2[1\cdots j]$. We do not here subcategorize $\text{Dup}$ as with $E$ and $F$. Our new recurrence in the DSI model is therefore:

  

$$

S[i,j] = \max 
\begin{cases}



&\text{Dup}[i; j]\\

&E[i, j]\\

&F[i, j]\\

&M[i, j]\\

&0
\end{cases}
$$
The remainder of the paper solves the following problem:

**Local Alignment with Tandem Duplication**

**Input**: Strings $S_{1}[1\cdots m]$ and $S_{2}[1\cdots n]$ each containing zero, one or more occurrences of tandem repeats interspersed with regions that are not tandem repeats. 

**Output**: Best scoring *local* alignment of $S_{1}$ and $S_{2}$ under the $\text{DSI}$ model. 

inthe following sections, we handle the problem of efficiently computing $\text{Dup}$ to obtain biologically meningful alignments. Our main result concerns a modification of Wraparound Dynamic Programming (WDP). WDP is explained more fully in Appendix A. We are concerned especially with GWDP, global alignment using WDP, a full recurrence of which is provided in Appendix B. 
  

---

  

## An Exact Duplication Alignment Algorithm

  

In this section, we investigate the time and space complexity of two exact algorithms for producing alignments under the DSI model. We make the following assumptions about mutation type and order:

  

**Assumption 1:** There are no excisions (removal of a copy from a tandem repeat region).

  

**Assumption 2:** Duplication occurs before other types of mutations (indels and substitutions).

  

These assumptions simplify the biology, but they do not affect the general alignment of the sequences so much as they affect the score of the alignment. That is, we are seeking an algorithm that recognizes a large gap as the result of a duplication rather than an indel. Even if these assumptions force the wrong copy of the tandem repeat pattern to be duplicated in the alignment, the dynamic programming algorithms, when presented with a large contiguous gap, will preferentially select the modest number of mismatch and indel errors required for duplication and alignment rather than select a large indel. Elimination of these assumptions would require confounding alignment with the much more complicated problem of tracing the evolutionary history of a tandem repeat and specically with respect to Assumption 2 would require aligning hypothetical sequences

  

### Including the Duplication Option

  

Given two strings $S_1$ and $S_2$, at each index pair $(y,x)$, we want to find the maximum score obtained by a duplication. Let $U$ be a substring that is duplicated and let $T$ be the substring aligned with the duplicate copies of $U$ ($|U| = k$, $|T| = h$). We are interested in computing the following:

  

$$

\text{Dup}[y,x] = \max_{U,T} 

\begin{cases} 
S[y - h, x - k] &+ \gamma + \text{dupcost}(U, T) - \delta,\\
&T = S_1[y - h + 1, \ldots, y]\\
&U = S_2[x - k + 1, \ldots, x]\\
S[y - k, x - h] &+ \alpha + \text{dupcost}(U; T) - \beta,\\
&U = S_1[y - k + 1, \ldots, y]\\
&T = S_2[x - h + 1, \ldots, x]\\
\end{cases}


$$

  

where both $\alpha$ and $\beta$ are negative valued parameters. In order to avoid redundancy, throughout the remainder of this paper, we will assume that $U$ comes from $S_2$ and $T$ comes from $S_1$. Dup is an alignment score based on the premise that the alignment ends with a tandem duplication of $U$ aligned with $T$. Here, $S[i; j]$ is a local alignment score under the DSI model, $\alpha$ is a duplication initiation cost, and $\text{dupcost}(U; T)$ is the maximum alignment score for duplicating and aligning substring $U$ with substring $T$. It is composed of:

  

1. A duplication extension cost $\beta$, for each copy of $U$ used in the alignment, and

  

2. An SI model cost to transform the copies of $U$ into $T$.

  

### Including the Duplication Option (Continued)

  

The **dupcost** function is defined as follows:

  

$$

\text{dupcost}(U; T) = \begin{cases}

\text{SI\_score}(U, T) + \beta \cdot (\text{num\_copies} - 1), & \text{if } \text{num\_copies} > 1 \\

\text{SI\_score}(U, T), & \text{if } \text{num\_copies} = 1

\end{cases}

$$

  

Here, **SI\_score** is the alignment score computed using the SI model, and **num\_copies** is the number of tandem copies of $U$ aligned with $T$. The term $\beta \cdot (\text{num\_copies} - 1)$ accounts for the duplication extension cost, ensuring that only additional copies beyond the first are penalized.

  

### Algorithm 1: Exact Duplication Alignment Using GWDP

  

To compute the duplication alignment score efficiently, we use a modified version of **Global Wraparound Dynamic Programming (GWDP)**. This algorithm aligns a substring $U$ with multiple tandem copies of another substring $T$.

  

The recurrence relation for GWDP is modified to include the duplication initiation cost $\alpha$ and extension cost $\beta$:

  

$$

G[i, j] = \max \left\{

\begin{aligned}

&G[i-1, j-1] + \text{match}(S_1[i], S_2[j]) && \text{(diagonal match)} \\

&G[i-1, j] + \beta && \text{(insertion in } S_2 \text{)} \\

&G[i, j-1] + \beta && \text{(insertion in } S_1 \text{)} \\

&G[i-k, j] + \alpha + \beta \cdot (\text{num\_copies} - 1) && \text{(duplication of } U \text{)}

\end{aligned}

\right.

$$

  

Here, $k$ is the length of the substring $U$, and **num\_copies** is determined by the alignment.

  

### Time and Space Complexity

  

The time complexity of this algorithm is **O(m³)**, where $m$ is the length of the sequences. This is because we need to compute the GWDP matrix for each possible substring $U$ and $T$. The space complexity is **O(m²)**, as we need to store the GWDP matrix for each pair of substrings.

  

---

  

## A Heuristic Alignment Algorithm

  

To improve the efficiency of the exact algorithm, we propose a heuristic approach that reduces the number of substrings $U$ and $T$ considered, as well as the number of index pairs $(y, x)$ where duplication costs are computed.

  

### Limiting the (y; x) pairs that compute dupcost

  

We restrict the computation of **dupcost** to regions that are likely to contain tandem repeats. This is achieved by:

  

1. **Detecting tandem repeat regions**: Using a heuristic algorithm to identify tandem repeat regions in both sequences.

  

2. **Identifying duplication candidates**: For each tandem repeat region in $S_1$, we find candidate regions in $S_2$ that align well with the consensus sequence of the repeat unit.

  

### Restricting the number and size of candidate substrings U

  

Once a tandem repeat region is identified, we assume that duplications occur in units of the repeat size $k$. This allows us to focus on substrings $U$ of length $k$ or close to $k$, significantly reducing the number of candidates.

  

### Heuristic Algorithm Framework

  

The heuristic algorithm follows this framework:

  

1. **Preprocessing**:

  

- Detect tandem repeat regions in both sequences using a heuristic algorithm.

  

- For each tandem repeat region, find candidate regions in the other sequence that align well with the consensus sequence.

  

1. **Alignment**:

  

- For each index pair $(y, x)$ within tandem repeat regions, compute the duplication alignment score using a modified GWDP.

  

- Combine the duplication scores with the SI model scores to obtain the final alignment.

  

### Time and Space Complexity

  

The heuristic algorithm reduces the number of computations by focusing only on tandem repeat regions and their candidates. The time complexity is **O(m² + kNM)**, where $N$ is the total length of candidate regions, and $M$ is the length of the tandem repeat region. The space complexity is **O(m² + kN)** or **O(m + kN)** using optimized storage techniques.

  

## A Heuristic Alignment Algorithm (Continued)

  

### Detailed Steps of the Heuristic Algorithm

  

#### 1. Preprocessing Tandem Repeat Regions

  

To efficiently identify tandem repeat regions and their candidates, we use the following steps:

  

- **Detecting Tandem Repeats**: Use a heuristic algorithm to find tandem repeats in both sequences $S_1$ and $S_2$. This algorithm should provide a consensus sequence for each tandem repeat region.

  

- **Finding Candidates**: For each tandem repeat region in $S_1$, use Local Wraparound Dynamic Programming (LWDP) to find candidate regions in $S_2$ that align well with the consensus sequence. Similarly, find candidates in $S_1$ for tandem repeats in $S_2$.

  

#### 2. Restricting Candidate Substrings $U$

  

Once tandem repeat regions and their candidates are identified, we restrict the candidate substrings $U$ to those that match the repeat unit size $k$:

  

- **Selecting Units**: For each candidate region $R$ in $S_2$ that aligns with a tandem repeat region in $S_1$, extract substrings $U$ of length $k$ or close to $k$. These substrings are used as potential duplication units.

  

- **Aligning Units**: Use the consensus sequence of the tandem repeat region to align with the candidate region $R$ and identify the best matching substrings $U$.

  

### Algorithm Implementation

  

The heuristic algorithm can be summarized as follows:

  

```markdown

**Algorithm: Heuristic Duplication Alignment**

  

**Input**: Sequences $S_1$ and $S_2$

  

**Output**: Best scoring alignment under the DSI model

  

**Steps**:

  

1. **Preprocess**:

  

- $L_1 = \text{TRScan}(S_1)$: Detect tandem repeats in $S_1$.

  

- For each tandem repeat region $TR$ in $L_1$:

  

- $TR.\text{candidates} = \text{LWDP}(TR.\text{consensus}, S_2)$

  

- $L_2 = \text{TRScan}(S_2)$

  

- For each tandem repeat region $TR$ in $L_2$:

  

- $TR.\text{candidates} = \text{LWDP}(TR.\text{consensus}, S_1)$

  

1. **Alignment**:

  

- Initialize the alignment score matrix $S$ using the SI model.

  

- For each index pair $(y, x)$ within tandem repeat regions:

  

- If $y$ is an index of a tandem repeat region in $L_1$ and $x$ is an index of a candidate in $L_1$:

  

- For each unit substring $U$ in the candidate list:

  

- Compute $\text{Dup}[y, x]$ using modified GWDP.

  

- Update $S[y, x]$ with the maximum score.

  

- Similarly, handle cases where $x$ is an index of a tandem repeat region and $y$ is an index of a candidate.

  

1. **Output**:

  

- Find the best score in $S$ and trace back to obtain the alignment.

```

  

### Time and Space Complexity (Continued)

  

- **Time Complexity**: The preprocessing step for detecting tandem repeats and finding candidates takes $O(m^2)$ time. The alignment step, which involves computing duplication scores for each candidate, takes $O(m^2 + kNM)$ time. Therefore, the overall time complexity is **$O(m^2 + kNM)$**.

  

- **Space Complexity**: The space complexity is dominated by storing the alignment score matrix $S$ and the GWDP matrices for each candidate. This results in a space complexity of **$O(m^2 + kN)$**. Using optimized storage techniques, the space complexity can be reduced to **$O(m + kN)$**.

  

---

  

## Examples

  

### Example 1: Alignment of Plasmodium chabaudi cDNA Sequences

  

We demonstrate the effectiveness of our algorithm using two samples of cDNA from the *Plasmodium chabaudi* erythrocyte membrane antigen mRNA. One sequence contains 20 copies of a 42 base pair pattern, while the other contains 19 copies. The alignment highlights the ability of our algorithm to bridge gaps caused by tandem duplications.

  

#### Alignment Schematic:

  

```

Sequence 1 (20 copies): ---------------------------------------------->

Sequence 2 (19 copies): -------------------------------> <gap>

```

  

#### Detailed Alignment:

  

```

Sequence 1: GTAATAGTAGA GAA CAA TG CTC CTG AAG AAA CT CAA GCC GAA ...

Sequence 2: gaaatagtaGA GAA CAA TG CTC CTG AAG AAA CT CAA GCC GAA ...

```

  

In this example, the gap in Sequence 2 is correctly identified as a tandem duplication event rather than an indel, demonstrating the advantage of our DSI model over the traditional SI model.

  

### Example 2: Alignment of Immunoglobulin κ Chain Genes

  

We also examine an intron region of the immunoglobulin κ chain genes in rat and mouse. Each sequence contains a different 101 bp tandem repeat. The alignment shows that our algorithm can handle complex tandem repeat structures and identify common elements between different repeats.

  

#### Alignment Schematic:

  

```

Mouse Sequence: -------------------------------> <gap> ----------------------------->

Rat Sequence: -------------------------------> <gap> ----------------------------->

```

  

#### Detailed Alignment:

  

```

Mouse: TCTGAGATA CCT CTG AGG ATC AC CAA TGG CAG AGT CG GCC AGC ACC TCA GC CTC CAG GCC ...

Rat: TCTGAGATA CCT CTG AGG AAC AC CAA TGG CAG AGT CA ACC AGC ACC TCA GC ctc cag act ...

```

  

In this case, the algorithm correctly identifies the duplicated regions and aligns them, demonstrating its robustness in handling complex biological sequences.

  

---

  

## Conclusion

  

We have demonstrated that incorporating tandem duplication into sequence alignment algorithms can significantly improve the accuracy of alignments, especially in regions with tandem repeats. Our exact and heuristic algorithms provide efficient solutions for aligning sequences under the DSI model. Given the prevalence of tandem repeats in genomic sequences, these algorithms will become increasingly important for accurate biological sequence analysis.

  

好的，以下是论文的第四部分，继续前面的内容：

  

---

  

## Conclusion (Continued)

  

### Significance and Future Work

  

The incorporation of tandem duplication into sequence alignment algorithms addresses a critical gap in current bioinformatics tools. Traditional alignment methods, such as those based solely on the SI model, often fail to accurately align sequences with tandem repeats. This can lead to incorrect inferences in evolutionary studies, gene regulation analyses, and disease-related research.

  

By introducing the DSI model and developing both exact and heuristic algorithms, we provide a more comprehensive framework for sequence alignment. This framework not only improves alignment accuracy but also enhances the biological relevance of the results. For instance, correctly identifying tandem duplications can reveal important insights into the mechanisms of genetic diseases and the functional roles of repetitive DNA elements.

  

### Future Directions

  

While our algorithms represent a significant advancement, there are several areas for future work:

  

1. **Handling Overlapping Repeats**: The current algorithms struggle with overlapping tandem repeats, as seen in the second example. Developing methods to handle such complex structures without violating the principle of optimality is a key challenge.

  

2. **Incorporating Additional Mutational Events**: The DSI model currently focuses on tandem duplication, substitutions, and indels. Future work could extend this model to include other mutational events, such as inversions and translocations, to provide an even more comprehensive alignment framework.

  

3. **Optimizing Computational Efficiency**: Although our heuristic algorithm significantly reduces computation time, further optimization is needed to handle very large genomic sequences efficiently. This could involve parallel computing techniques or more advanced data structures.

  

4. **Integration with Next-Generation Sequencing Data**: With the increasing volume of next-generation sequencing data, developing algorithms that can efficiently process these large datasets while accounting for tandem duplications will be crucial.

  

5. **Biological Validation**: Conducting more extensive biological validations using real-world datasets can further demonstrate the utility of our algorithms in practical applications, such as identifying disease-associated variants and understanding genome evolution.

  

---

  

## Acknowledgments

  

The author wishes to thank Astrid Jervis for her help in finding the mouse and rat sequences used in Example 2, and Richard Harlan for his help in finding the Plasmodium sequences used in Example 1. Their contributions were invaluable in demonstrating the practical applications of the algorithms presented in this paper.

  

---

  

## References

  

1. J. Armour, T. Anttinen, C. May, E. Vega, A. Sajantila, J. Kidd, K. Kidd, J. Bertranpetit, S. Pääbo, and A. Jeffreys. Minisatellite diversity supports a recent African origin for modern humans. *Nature Genetics*, 13:375–380, 1996.

  

2. G. Benson. A space-efficient algorithm for finding the best non-overlapping alignment score. *Theoretical Computer Science*, 147:277–290, 1995.

  

3. G. Benson. An algorithm for finding tandem repeats of unspecified pattern size. *Manuscript*, 1995.

  

4. G. Benson and M. Waterman. A method for fast database search for all k-nucleotide repeats. *Nucleic Acids Research*, 22:4826–4832, 1994.

  

5. V. Campuzano, L. Montermini, M.D. Molto, L. Pianese, and M. Cossee. Friedreich's ataxia: Autosomal recessive disease caused by an intronic GAA triplet repeat expansion. *Science*, 271:1423–1427, 1996.

  

6. A. Edwards, H. Hammond, L. Jin, C. Casky, and R. Chakraborty. Genetic variation at five trimeric and tetrameric tandem repeat loci in four human population groups. *Genomics*, 12:75–85, 1992.

  

7. V. Fischetti, G. Landau, J. Schmidt, and P. Sellers. Identifying periodic occurrences of a template with applications to a protein structure. In A. Apostolico, M. Crochemore, Z. Galil, and U. Manber, editors, *Proc. 3rd Annual Symp. on Combinatorial Pattern Matching*, Lecture Notes in Computer Science, volume 644, pages 200–211. Springer-Verlag, 1992.

  

8. Y.-H. Fu, A. Pizzuti, R.G. Fenwick Jr, J. King, S. Rajnarayan, P.W. Dunne, J. Dubel, G.A. Nasser, T. Ashizawa, P. DeJong, B. Wieringa, R. Korneluk, M.B. Perryman, H.F. Epstein, and C.T. Caskey. An unstable triplet repeat in a gene related to myotonic muscular dystrophy. *Science*, 260:1270–1272, 1993.

  

9. O. Gotoh. An improved algorithm for matching biological sequences. *J. Mol. Biol.*, 162:705–708, 1982.

  

10. H. Hamada, M. Seidman, B. Howard, and C. Gorman. Enhanced gene expression by the poly(dT-dG) poly(dC-dA) sequence. *Molecular and Cellular Biology*, 4:2629–2636, 1984.

  

11. L. Hellman, M. Steen, M. Sundvall, and U. Pettersson. A rapidly evolving region in the immunoglobulin heavy chain loci of rat and mouse: postulated role of (dC-dA)<sub>n</sub>(dG-dT)<sub>n</sub> sequences. *Gene*, 43:3–12, 1986.

  

12. D.S. Hirschberg. A linear space algorithm for computing longest common subsequences. *Communications of the ACM*, 18:341–343, 1975.

  

13. Huntington's Disease Collaborative Research Group. A novel gene containing a trinucleotide repeat that is expanded and unstable on Huntington's disease chromosomes. *Cell*, 72:971–983, 1993.

  

14. G. Landau and J. Schmidt. An algorithm for approximate tandem repeats. In *Proc. 4th Annual Symp. on Combinatorial Pattern Matching*, Lecture Notes in Computer Science, volume 684, pages 120–133. Springer-Verlag, 1993.

  

15. M-Y. Leung, B.E. Blaisdell, C. Burge, and S. Karlin. An efficient algorithm for identifying matches with errors in multiple long molecular sequences. *J. Mol. Biol.*, 221:1035–1049, 1991.

  

16. V.I. Levenshtein. Binary codes capable of correcting deletions, insertions, and reversals. *Soviet Phys. Dokl.*, 10:707–710, 1965.

  

17. Q. Lu, L. Wallrath, H. Granok, and S. Elgin. (CT)<sub>n</sub>(GA)<sub>n</sub> repeats and heat shock elements have distinct roles in chromatin structure and transcriptional activation of the Drosophila hsp70 gene. *Molecular and Cellular Biology*, 13:2802–2813, 1993.

  

18. W. Messier, S-H. Li, and C-B. Stewart. The birth of microsatellites. *Nature*, 365:2–3, 1993.

  

19. W. Miller and E. Myers. Approximate matching of regular expressions. *Bulletin of Mathematical Biology*, 53:5–14, 1991.

  

20. A. Milosavljević and J. Jurka. Discovering simple DNA sequences by the algorithmic significance method. *CABIOS*, 8:407–411, 1992.

  

21. S. Needleman and C. Wunsch. A general method applicable to the search for similarities in the amino acid sequence of two proteins. *J. Mol. Biol.*, 48:443–453, 1970.

  

22. M. Pardee, K. Lowenhaupt, A. Rich, and A. Nordheim. (dC-dA)<sub>n</sub>(dG-dT)<sub>n</sub> sequences have evolutionarily conserved chromosomal locations in Drosophila with implications for roles in chromosome structure and function. *The EMBO Journal*, 7:1817–1825, 1988.

  

23. R. Richards, K. Holman, S. Yu, and G. Southerland. Fragile X syndrome unstable element, p(CCG)<sub>n</sub>, and other simple tandem repeat sequences are binding sites for specific nuclear proteins. *Hum. Mol. Genet.*, 1:581–586, 1992.

  

24. J.P. Schmidt. All highest scoring paths in weighted grid graphs and its application to finding all approximate repeats in strings. In *Third Israel Symposium on Theory of Computing and Systems*, pages 77–87. IEEE Computer Society Press, 1995.

  

25. M. Schöniger and M. Waterman. A local algorithm for DNA sequence alignment with inversions. *Bulletin of Mathematical Biology*, 54:521–536, 1992.

  

26. P. Sellers. An algorithm for the distance between two sequences. *J. Comb. Theory*, 16:253–258, 1974.

  

27. T. Smith and M. Waterman. Comparison of biosequences. *Advances in Applied Mathematics*, 2:482–489, 1981.

  

28. S.A. Tishkoff, E. Dietzsch, W. Speed, A.J. Pakstis, and J.R. Kidd. Global patterns of linkage disequilibrium at the CD4 locus and modern human origins. *Science*, 271:1380–1387, 1996.

  

29. A. Verkerk, M. Pieretti, J. Sutcliffe, Y. Fu, D. Kuhl, A. Pizzuti, O. Reiner, S. Richards, M. Victoria, F. Zhang, B. Eussen, G. van Ommen, A. Blonden, G. Riggins, J. Chastain, C. Kunst, H. Galjaard, C. Casky, D. Nelson, B. Oostra, and S. Warren. Identification of a gene (FMR-1) containing a CGG repeat coincident with a breakpoint cluster region exhibiting length variation in fragile X syndrome. *Cell*, 70:1069–1078, 1992.

  

30. J. Weber and P. May. Abundant class of human DNA polymorphisms which can be typed using the polymerase chain reaction. *Am. J. Hum. Genet.*, 44:388–396, 1989.

  

31. H. Yee, A. Wong, J. van den Sande, and J. Rattner. Identification of novel single-stranded d(TC)<sub>n</sub> binding proteins in several mammalian species. *Nucleic Acids Res.*, 23:2222–2228, 1995.

  

---

  

## Appendices

  

### Appendix A: Wraparound Dynamic Programming

  

The ability to detect and align tandem repeats has been greatly facilitated by the development of Wraparound Dynamic Programming (WDP). This method was first introduced by Myers and Miller [10] and independently developed and simplified by Fischetti, Landau, Schmidt, and Sellers [7]. The technique is applicable to both local (LWDP) and global (GWDP) sequence alignment.

  

Let $A$ and $B$ be two strings with lengths $n$ and $k$ respectively, where $k$ can be much smaller than $n$. The strength of WDP lies in its ability to align $A$ with an unknown number of tandem copies of $B$ using a matrix of size $nk$ rather than $n^2$. The technique involves a normal similarity computation, but the cell values in each row are computed with two passes through the row.

  

For **LWDP**, in the first pass, cell $S[i, 1]$ (corresponding to $A_i$ and $B_1$) is given the maximum of:

  

1. A value derived from the cell $S[i-1, 1]$, the first cell in the row above.

  

2. A value derived from cell $S[i-1, k]$, the last cell in the row above.

  

3. Zero.

  

For **GWDP**, cell $S[i, 1]$ is given the maximum of:

  

1. A value derived from the cell $S[i-1, 1]$.

  

2. A value derived from cell $S[i-1, k]$.

  

3. A value derived from deleting the first $i-1$ characters of $A$.

  

4. A value derived from deleting the first $i$ characters of $A$.

  

For both LWDP and GWDP, in the second pass, $S[i, 1]$ receives the maximum of:

  

1. Its current value.

  

2. A value derived from $S[i, k]$, the last cell in its row.

  

Thus, in both the first and second passes, the computation wraps around.

  

---

  

### Appendix B: Global Wraparound Dynamic Programming

  

Global Wraparound Dynamic Programming (GWDP) is used to calculate the best global alignment score between a sequence $T$ and tandem copies of a sequence $U$. In particular, let $G$ be the computation array and let $|U| = k$. Then the complete recurrence is:

  

**Initialization**:

  

$$

\begin{aligned}

G[0, 0] &= 0 \\

G[0, j] &= G[0, 0] + \alpha +j \beta \quad \text{(for } j > 0\text{)}

\end{aligned}

$$

  

**Initialization of row $i$**:

  

$$

\begin{aligned}

G[i, 0] &= G[0, 0] + \alpha + i \beta \quad \text{(for } i > 0\text{)}

\end{aligned}

$$

  

**Pass 1** ($i \geq 1$, $j \geq 1$):

  

$$

\begin{aligned}

E[i, j] &= \begin{cases}

G[i, 0] + \alpha + \beta j, & \text{if } j = 1 \\

\max \left\{ E[i, j-1] + \beta, G[i, j-1] + \alpha + \beta \right\}, & \text{if } j > 1

\end{cases} \\

F[i, j] &= \begin{cases}

G[0, j] + \alpha + \beta i, & \text{if } i = 1 \\

\max \left\{ F[i-1, j] + \beta, G[i-1, j] + \alpha + \beta \right\}, & \text{if } i > 1

\end{cases} \\

G[i, j] &= \begin{cases}

\max \left\{ G[i-1, 0] + \text{match}(T_i, U_1), F[i, 1], E[i, 1] \right\}, & \text{if } j = 1 \\

\max \left\{ G[i-1, j-1] + \text{match}(T_i, U_j), F[i, j], E[i, j] \right\}, & \text{if } j > 1

\end{cases}

\end{aligned}

$$

  

**Pass 2** ($i \geq 1$, $j \geq 1$):

  

$$

\begin{aligned}

E[i, j] &= \begin{cases}

\max \left\{ E[i, k] + \beta, G[i, k] + \alpha + \beta \right\}, & \text{if } j = k \\

\max \left\{ E[i, j-1] + \beta, G[i, j-1] + \alpha + \beta \right\}, & \text{if } j > k

\end{cases} \\

G[i, j] &= \max \left\{ G[i, j], E[i, j] \right\}

\end{aligned}

$$

  

---

  

### Appendix C: GWDP for Fixed $U$

  

The following recursion computes:

  

$$

D[y] = \max_{0 < h \leq y} \left\{ S[y - h, x - k] + \alpha + \text{dupcost}(U, T) - \beta \right\}

$$

  

where $U = S_2[x - k + 1: x]$, $|U| = k$, and $T = S_1[y - h + 1: y]$, $|T| = h$. $D[y]$ is the best local alignment score for suffixes of $S_1[1:y]$ and $S_2[1:x]$ given that the alignment ends with duplication of a substring $U$ to match a substring $T$.

  

**Initialization**:

  

$$

\begin{aligned}

G[0, 0] &= X_0 \\

G[0, j] &= G[0, 0] + \alpha + j \beta + I(j = k) \gamma \quad \text{(for } j > 0\text{)}

\end{aligned}

$$

  

**Initialization of row $i$**:

  

$$

\begin{aligned}

F[i, 0] &= \begin{cases}

G[0, 0] + \alpha + \beta i, & \text{if } i = 1 \\

\max \left\{ F[i-1, 0] + \beta, G[i-1, 0] + \alpha + \beta \right\}, & \text{if } i > 1

\end{cases} \\

G[i, 0] &= \max \left\{ X_i \text{ (reinitialize)}, F[i, 0] \right\}

\end{aligned}

$$

  

**Pass 1** ($i \geq 1$, $j \geq 1$):

  

$$

\begin{aligned}

E[i, j] &= \begin{cases}

G[i, 0] + \alpha + \beta + I(j = k) \gamma, & \text{if } j = 1 \\

\max \left\{ E[i, j-1] + \beta + I(j = k) \gamma, G[i, j-1] + \alpha + \beta + I(j = k) \gamma \right\}, & \text{if } j > 1

\end{cases} \\

F[i, j] &= \begin{cases}

G[0, j] + \alpha + \beta i, & \text{if } i = 1 \\

\max \left\{ F[i-1, j] + \beta, G[i-1, j] + \alpha + \beta \right\}, & \text{if } i > 1

\end{cases} \\

G[i, j] &= \begin{cases}

\max \left\{ G[i-1, 0] + \text{match}(T_i, U_1) + I(j = k) \gamma, F[i, 1], E[i, 1] \right\}, & \text{if } j = 1 \\

\max \left\{ G[i-1, j-1] + \text{match}(T_i, U_j) + I(j = k) \gamma, F[i, j], E[i, j] \right\}, & \text{if } j > 1

\end{cases}

\end{aligned}

$$

  

**Pass 2** ($i \geq 1$, $j \geq 1$):

  

$$

\begin{aligned}

E[i, j] &= \begin{cases}

\max \left\{ E[i, k] + \beta + I(j = k) \gamma, G[i, k] + \alpha + \beta + I(j = k) \gamma \right\}, & \text{if } j = k \\

\max \left\{ E[i, j-1] + \beta + I(j = k) \gamma, G[i, j-1] + \alpha + \beta + I(j = k) \gamma \right\}, & \text{if } j > k

\end{cases} \\

G[i, j] &= \max \left\{ G[i, j], E[i, j] \right\}

\end{aligned}

$$

  

---

  

### Appendix D: Algorithm 3

  

**Algorithm 3: Heuristic Duplication Alignment**

  

**Input**: Sequences $S_1$ and $S_2$

  

**Output**: Best scoring alignment of $S_1$ and $S_2$ under the DSI model with heuristic determination of tandem repeat regions and candidates for duplication.

  

**Steps**:

  

1. **Preprocess**:

  

```markdown

L_1 = TRScan(S_1)

for TR in L_1:

TR.candidates = LWDP(TR.consensus, S_2)

L_2 = TRScan(S_2)

for TR in L_2:

TR.candidates = LWDP(TR.consensus, S_1)

```

  

1. **Alignment**:

  

```markdown

for y = 1 to length(S_1):

for x = 1 to length(S_2):

S[y][x] = max{SI model scores}

if (y is an index of some TR in L_1) AND (x is an index of some C in TR.candidates):

for each unit substring U in C[x].unit-list:

DupS_2 = DupOption(U, TR, y)

S[y][x] = max{S[y][x], DupS_2}

if (x is an index of some TR in L_2) AND (y is an index of some C in TR.candidates):

for each unit substring U in C[y].unit-list:

DupS_1 = DupOption(U, TR, x)

S[y][x] = max{S[y][x], DupS_1}

```

  

1. **Output**:

  

```markdown

Find best score S[y][x], trace back and output alignment.

```

  

---

  

### Figures

  

#### Figure 1: Duplication Option

  

![Duplication Option](figure1.png)

  

*The duplication option permits score $S[i, j]$ to represent an alignment ending with a duplication of the right end of one sequence aligned with the right end of the other.*

  

---

#### Figure 2: Calculations for Duplication Alignment

  

![Calculations for Duplication Alignment](figure2.png)

  

*Calculations for $ \text{Dup}[y, x] $ for a fixed duplication unit $ U $ and two substrings $ T $ and $ T' $. The heavy lines in the arrays represent the alignments illustrated on the right. The calculations can be merged so that only the best alternative is computed by setting $ G[i][0] $ equal to the maximum of $ S[i, x - k] + \alpha - \beta $ and the best score that $ G[i][0] $ receives from the rows above.*

  

---

  

#### Figure 3: Alignment of Plasmodium chabaudi Sequences

  

![Alignment of Plasmodium chabaudi Sequences](figure3.png)

  

*A schematic of the alignment of two versions of cDNA from the *Plasmodium chabaudi* erythrocyte membrane antigen mRNA. The upper sequence contains 20 copies of a 42 nucleotide pattern, and the lower sequence contains 19 copies. An SI model local alignment algorithm was not able to bridge the gap with a long indel.*

  

---

  

#### Figure 4: Detailed Alignment of Plasmodium chabaudi Sequences

  

![Detailed Alignment of Plasmodium chabaudi Sequences](figure4.png)

  

*Alignment of Plasmodium chabaudi sequences. Only the left end of the alignment is shown, containing the final six copies of the tandem repeat. Gaps due to duplication are indicated by lowercase letters in the sequence that contains the gap.*

  

---

  

#### Figure 5: Alignment of Immunoglobulin κ Chain Genes

  

![Alignment of Immunoglobulin κ Chain Genes](figure5.png)

  

*A schematic of the alignment of an intron region in rat and mouse immunoglobulin κ chain genes. The rat contains two copies of one repeat, and the mouse contains two copies of another. The two duplicated regions are the same length. The overlap is 20 nucleotides.*

  

---

  

#### Figure 6: Detailed Alignment of Immunoglobulin κ Chain Genes

  

![Detailed Alignment of Immunoglobulin κ Chain Genes](figure6.png)

  

*Alignment of an intron region in rat and mouse immunoglobulin κ chain genes. Gaps due to duplication are indicated by lowercase letters in the sequence that contains the gap. The duplicated sequence is indicated by the symbols `<------------------>`. The alignment shows that the duplicated subsequences have the same length and contain a common element of 20 characters.*

  

---

  

## Conclusion

  

We have demonstrated that wraparound dynamic programming can be efficiently incorporated into a local alignment algorithm to align sequences containing tandem repeats. Given the abundance of tandem repeats in genomic sequences, finding them and aligning sequences containing them will become an increasingly common objective. Our algorithms provide a robust framework for handling tandem repeats, improving the accuracy and biological relevance of sequence alignments.

  

---

  

## Acknowledgments

  

The author wishes to thank Astrid Jervis for her help in finding the mouse and rat sequences used in Example 2, and Richard Harlan for his help in finding the Plasmodium sequences used in Example 1. Their contributions were invaluable in demonstrating the practical applications of the algorithms presented in this paper.