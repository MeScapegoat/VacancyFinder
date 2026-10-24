#include <gtest/gtest.h>

#include <QFile>

#include <LinkedInParserStrategy/LinkedInDetailsJsonVacancyExtractor.h>

TEST(LinkedInDetailsJsonVacancyExtractor, NoJson)
{
    QFile file(":/html_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const QJsonObject json = extractJsonObject(html);

    ASSERT_TRUE(json.isEmpty());
}

TEST(LinkedInDetailsJsonVacancyExtractor, ExtractJson)
{
    QFile file(":/json_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const QJsonObject json = extractJsonObject(html);

    ASSERT_FALSE(json.isEmpty());
}

TEST(LinkedInDetailsJsonVacancyExtractor, Json1)
{
    QFile file(":/json_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QUrl url("json 1 url");

    const QString html = file.readAll();
    const QJsonObject json = extractJsonObject(html);

    const LinkedInDetailsJsonVacancyExtractor extractor(json, url);

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "Senior Software Engineer (C++) for JVM runtime");
    EXPECT_EQ(vacancy.url, url);
    EXPECT_EQ(vacancy.companyName, "Azul");
    EXPECT_EQ(vacancy.companyUrl, QUrl("https://www.linkedin.com/company/azul-systems"));
    EXPECT_EQ(vacancy.location, "Belgrade");
    EXPECT_EQ(vacancy.employmentType, "FULL_TIME");

    EXPECT_TRUE(vacancy.skills.isEmpty());

    EXPECT_EQ(vacancy.description, "<p>We are seeking a highly skilled **Senior Software Engineer** to join our team developing Azul Prime, an award-winning enhanced build of OpenJDK designed for superior application performance, responsiveness, and efficiency. In this role, you will drive the development and optimization of the virtual machine (VM), contributing to garbage collection, HotSpot runtime, compiler runtime, remote compilation, and many other advanced challenges.</p><p><p><br></p><strong>What You'll Do</strong><ul><br><li>Design, develop, maintain, and optimize high‑performance, low‑level C++ code for system‑critical JVM runtime components and distributed client‑server communication using gRPC, HTTP/2, or HTTP, ensuring minimal overhead, robustness, and reliability at scale.<br><br></li><br><li>Lead the delivery of complex technical projects — from design through production — taking ownership of outcomes and navigating ambiguity under real time and resource constraints.<br><br></li><br><li>Contribute to the technical direction of the team: participate in design discussions, challenge and pressure-test technical approaches, and ensure robust solutions through constructive peer review.<br><br></li><br><li>Collaborate across teams — represent the team's technical interests in cross-team initiatives and contribute as a peer in design and direction discussions with partner engineering groups.<br><br></li><br><li>Mentor junior engineers, sharing expertise through reviews, pairing, and practical guidance that raises the quality bar across the team.<br><br></li><br><li>Explore and adopt AI-first workflows — proactively identifying where AI tools can enhance productivity, code quality, and development processes, and helping teammates adopt effective practices.<br><br></li><br><li>Perform detailed debugging, profiling, and root-cause analysis of complex software issues.<br><br></li><br><li>Develop comprehensive tests to ensure high quality and demonstrate performance improvements.</li><br></ul><p><br></p><strong>Wha you'll Need</strong><ul><br><li>Bachelor's degree in Computer Science or a related field, and a minimum of 5 years of experience writing low‑level, high‑performance C++ code.<br><br></li><br><li>Solid knowledge of VM and compiler runtime design and implementation; familiarity with JVM internals is highly desirable.<br><br></li><br><li>Demonstrated ability to independently lead and deliver complex technical projects on time, managing competing priorities and technical uncertainty.<br><br></li><br><li>Experience mentoring engineers and contributing to a team's technical culture and practices.<br><br></li><br><li>Strong proficiency with low‑level programming concepts — memory management, pointers, concurrency, and multithreading.<br><br></li><br><li>Experience using debuggers, profilers, and performance analysis tools to investigate and resolve complex problems.<br><br></li><br><li>Understanding of hardware architectures (e.g. x86, ARM), operating system internals (e.g. Linux, macOS), and languages such as Java or Rust is an advantage.</li><br></ul><p><br></p><strong>We Offer</strong><br><ul><br><li><br><p>Referral Program — help us grow with great people.</p><br></li><br><li><br><p>Premium IT equipment — MacBook Pro or hardware of your choice.</p><br></li><br><li><br><p>5 weeks of holidays + 5 sick days.</p><br></li><br><li><br><p>Flexible working hours; full remote possible.</p><br></li><br><li><br><p>Prague/Limassol/Belgrade city center offices. Hybrid or full-remote also possible.</p><br></li><br><li><br><p>Work alongside top experts worldwide contributing to the Java ecosystem.</p><br></li><br></ul><br></p><p><br></p><br><p>Employment at Azul is contingent upon the completion of reference and background checks.</p><br><p>We may use artificial intelligence (AI) tools to support parts of the hiring process, such as reviewing applications, analyzing resumes, or assessing responses and identifying potential inconsistencies or verification signals in application materials based on available information. These tools assist our recruitment team but do not replace human judgment. Final hiring decisions are ultimately made by humans. If you would like more information about how your data is processed, please contact us.</p>");
    EXPECT_EQ(vacancy.date, QDate::fromString("2026-09-27", Qt::ISODate));
    EXPECT_EQ(vacancy.industry.value(), "Software Development,Technology, Information and Media,IT Services and IT Consulting");

    EXPECT_FALSE(vacancy.level.has_value());

    EXPECT_EQ(vacancy.experience.value(), 60);
    EXPECT_EQ(vacancy.education.value(), "bachelor degree");
    EXPECT_EQ(vacancy.expirationDate.value(), QDate::fromString("2026-12-08", Qt::ISODate));
}

TEST(LinkedInDetailsJsonVacancyExtractor, Json2)
{
    QFile file(":/json_2.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QUrl url("json 2 url");

    const QString html = file.readAll();
    const QJsonObject json = extractJsonObject(html);

    const LinkedInDetailsJsonVacancyExtractor extractor(json, url);

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "C++ Software Engineer – SDK System & Performance");
    EXPECT_EQ(vacancy.url, url);
    EXPECT_EQ(vacancy.companyName, "Cisco");
    EXPECT_EQ(vacancy.companyUrl, QUrl("https://www.linkedin.com/company/cisco"));
    EXPECT_EQ(vacancy.location, "Belgrade");
    EXPECT_EQ(vacancy.employmentType, "FULL_TIME");

    EXPECT_TRUE(vacancy.skills.isEmpty());

    EXPECT_EQ(vacancy.description, "<strong>Meet the Team<br><br></strong>We, at CISCO Silicon One™, are building SDK, firmware, simulator, and compiler for the top-of-the-line Networking ASIC.<br><br>The SDK System & Performance team manages all of SDK features that are device agnostic. This involves intricate C++ projects, that requires proficiency and intuition of C++, compiler, linker, loader, and general Linux program runtime environment and subsystems. You will improve SDK code quality, manage SDK threads, and various common SDK tools and utilities.<br><br>We manage SDK memory footprint and debuggability, which involves deep understanding of the Linux memory subsystem. You will profile, analyse, propose and implement solutions to improve SDK memory usage, and make it more robust.<br><br>One of the major cross device functionalities and engineering challenges is supporting SDK critical updates on live systems. You will work closely with major CISCO customers to enable and validate these functionalities.<br><br>We are looking for people who are in love with technology and engineering to help us push the limits of what is possible through innovation, determination, and teamwork.<br><br><strong>Your Impact<br><br></strong><ul><li>Design and deliver core, cross-device SDK infrastructure in modern C++ (C++14/C++17), with a strong focus on performance, scalability, and long-term maintainability</li><li>Drive system-level engineering across the build and runtime stack, including deep work with the compiler, linker, loader, and binary/layout considerations</li><li>Own and improve the SDK’s memory footprint and debuggability, building mechanisms that increase observability and reliability in production environments</li><li>Diagnose complex issues across compile-time and runtime (performance regressions, memory behaviour, and hard-to-reproduce failures) and implement durable fixes</li><li>Develop and maintain low-level tooling and debug layers (e.g., allocation tracking/interposition) to support troubleshooting and serviceability</li><li>Collaborate with partner teams to shape SDK architecture, design patterns, and engineering practices that enable robust platform operation and critical update workflows<br><br></li></ul>The result of your work will be integrated into the products of world-leading companies and will have a worldwide impact.<br><br><strong>Minimum Qualifications<br><br></strong><ul><li>3+ years of programming experience in C++ and system programming</li><li>Solid knowledge of Linux</li><li>Academic education in computer technology (finished BSc/MSc studies)</li><li>Fluency in spoken and written English<br><br></li></ul><strong>Preferred Qualifications<br><br></strong><ul><li>Solid understanding of hardware architecture and its interaction with software.</li><li>Passion for SW architecture.</li><li>Good problem-solving, debugging, and analytical skills.</li><li>Experience with other programming languages and tools such as Python.<br><br></li></ul>Why Cisco?<br><br>At Cisco, we’re revolutionizing how data and infrastructure connect and protect organizations in the AI era – and beyond. We’ve been innovating fearlessly for 40 years to create solutions that power how humans and technology work together across the physical and digital worlds. These solutions provide customers with unparalleled security, visibility, and insights across the entire digital footprint.<br><br>Fueled by the depth and breadth of our technology, we experiment and create meaningful solutions. Add to that our worldwide network of doers and experts, and you’ll see that the opportunities to grow and build are limitless. We work as a team, collaborating with empathy to make really big things happen on a global scale. Because our solutions are everywhere, our impact is everywhere.<br><br>We are Cisco, and our power starts with you.");

    EXPECT_EQ(vacancy.date, QDate::fromString("2026-09-24", Qt::ISODate));
    EXPECT_EQ(vacancy.industry.value(), "Software Development");

    EXPECT_FALSE(vacancy.level.has_value());

    EXPECT_EQ(vacancy.experience.value(), 36);
    EXPECT_EQ(vacancy.education.value(), "bachelor degree");
    EXPECT_EQ(vacancy.expirationDate.value(), QDate::fromString("2026-12-14", Qt::ISODate));
}